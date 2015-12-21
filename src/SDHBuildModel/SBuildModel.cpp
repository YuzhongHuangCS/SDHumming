/*
* SBuildModel.cpp  SDHumming Model Building routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/

#include "SBuildModel.h"
#include <iostream>
#include <fstream>
#include "midifile.h"
#define PHRASE_SEGMENTATION

using namespace std;

int BuildSDHummingModel::ReadFromNotes(vector<MIDIINFO> myNotes){
	int i=0;
	int ValidNoteCount=0;
	TotalNoteNumber=myNotes.size();
	pMidiNoteStruct=new NoteStructure[TotalNoteNumber];
	
	for(i=0;i<TotalNoteNumber;i++){
		if(myNotes[i].Durationms!=0){
			pMidiNoteStruct[ValidNoteCount].iNoteStartTime=myNotes[i].Startms;
			pMidiNoteStruct[ValidNoteCount].iDurationms=myNotes[i].Durationms;
			pMidiNoteStruct[ValidNoteCount].iNote=myNotes[i].type;
			pMidiNoteStruct[ValidNoteCount].itrackNo=myNotes[i].Channel;
			if(myNotes[i].velocity==5)
				pMidiNoteStruct[ValidNoteCount].iPhraseTag=1;
			else
				pMidiNoteStruct[ValidNoteCount].iPhraseTag=0;
			ValidNoteCount++;
		}
	}

	TotalNoteNumber=ValidNoteCount;
	for(i=0;i<TotalNoteNumber-1;i++){
		pMidiNoteStruct[i].iDurationWithRest=pMidiNoteStruct[i+1].iNoteStartTime-pMidiNoteStruct[i].iNoteStartTime;
	}
	pMidiNoteStruct[TotalNoteNumber-2].iDurationWithRest=pMidiNoteStruct[TotalNoteNumber-2].iDurationms;
	
	return 0;
}

vector<MIDIINFO> BuildSDHummingModel::LoadMidiFile(char* szInFile){ 
	MidiFile_t myMidi = MidiFile_load(szInFile);
	
	int nTracks=MidiFile_getNumberOfTracks(myMidi);
	MidiFileTrack_t curTrack = MidiFile_getFirstTrack(myMidi);
	int eType=0;
	int isNoteStartEvent=0;
	int nCount=0;
	vector<MIDIINFO> NoteVec;
	int CountStart=0;
	int CountEnd=0;
	while(curTrack){
		MidiFileEvent_t eEvent=MidiFileTrack_getFirstEvent(curTrack);	
		while(eEvent){
			eEvent=MidiFileEvent_getNextEventInTrack(eEvent);
			eType = MidiFileEvent_getType(eEvent);
			isNoteStartEvent=MidiFileEvent_isNoteStartEvent(eEvent);
			int isNoteEndEvent=MidiFileEvent_isNoteEndEvent(eEvent);
			if(eType==1){
				if(isNoteStartEvent==1){
					CountStart++;
					MIDIINFO tmp;
					tmp.Durationms=0;
					tmp.type=MidiFileNoteOnEvent_getNote(eEvent);
					tmp.Startms=(int)(MidiFile_getTimeFromTick(myMidi, MidiFileEvent_getTick(eEvent))*1000);
					tmp.Channel=MidiFileNoteOnEvent_getChannel(eEvent)+1;
					tmp.velocity=MidiFileNoteOnEvent_getVelocity(eEvent);
					NoteVec.push_back(tmp);
				}

				if(isNoteEndEvent==1){
					int _trackNo=MidiFileNoteOnEvent_getChannel(eEvent)+1;
					int _note=MidiFileNoteOnEvent_getNote(eEvent);
					int _startTime=(int)(MidiFile_getTimeFromTick(myMidi, MidiFileEvent_getTick(eEvent))*1000);
					int BestOffset=-1;
					int BestPos=-1;
					for(int x=0;x<NoteVec.size();x++){
						if(NoteVec[x].Channel == _trackNo && NoteVec[x].type == _note 
							&& _startTime>NoteVec[x].Startms && NoteVec[x].Durationms==0){
							NoteVec[x].Durationms=(int)(MidiFile_getTimeFromTick(myMidi, MidiFileEvent_getTick(eEvent))*1000) - NoteVec[x].Startms;
							break;
						}
					}

				}
			}
		}
		curTrack = MidiFileTrack_getNextTrack(curTrack);
	}
	
	return NoteVec;
}

int BuildSDHummingModel::Write2Model(){
	char NewModelFile[1024]="";
	char ModelIDTableFile[1024]="";
	strcpy(NewModelFile,m_pszModelDir);
	strcpy(ModelIDTableFile,m_pszModelDir);
	strcat(NewModelFile,"\\QBH.Model");
	strcat(ModelIDTableFile,"\\QBHModel.info");
	
	multimap<int, MidiMetaInfo>::iterator Iter;
	for (Iter=m_FileTrackmap.begin();Iter!=m_FileTrackmap.end();++Iter){
		vector<MIDIINFO> myNotes=LoadMidiFile((char*)Iter->second.MidiFilename.c_str());
		if(myNotes.size()==0){
			printf("file cannot be opened: %s, please check\n",(char*)Iter->second.MidiFilename.c_str());
			return -1;
		}
		ReadFromNotes(myNotes);
		if(!ExtractTrackNo(Iter->first,Iter->second.MetaInfo,(char*)Iter->second.MidiFilename.c_str()))
			return -1;
	}
	writeExtractedTrack(NewModelFile);
	writeSongInfo(ModelIDTableFile);
	return 0;
}

vector<string> ReadFromFile(char* filename){
	vector<string> AllStrs;
	ifstream infile;
	infile.open(filename,NULL);
	string str; 
	if(infile != NULL){
		while(getline(infile,str,'\n')) {
			AllStrs.push_back(str);
		}
	} 
	infile.close();
	return AllStrs; 
}

string GetSubstring(string s,int start,int end){
	return s.substr(start, end-start+1);
}

int ConvertMidilist(char* infile,char* outfile){
	vector<string> InStrVec=ReadFromFile(infile);
	FILE *fp=fopen(outfile,"wt");
	for(int i=0;i<InStrVec.size();i++){
		string subname;
		
		if((int)InStrVec[i].find_last_of('/')>0)
			subname=GetSubstring(InStrVec[i],InStrVec[i].find_last_of('/')+1,InStrVec[i].find_last_of('.')-1);
		else
			subname=GetSubstring(InStrVec[i],InStrVec[i].find_last_of('\\')+1,InStrVec[i].find_last_of('.')-1);
		fprintf(fp,"%s\t1\t%s\n",(char*)InStrVec[i].c_str(),(char*)subname.c_str());
	}
	fclose(fp);
	return 0;
}

int BuildSDHummingModel::GenFilelist(){
	int i=0,totalCount=0,countLine=0;
	int tempItem=0;
	
	string MidiFilename="";
	string MetaInfo="";
	int trackNo=0;
	ifstream InputModelFile;
	
	ConvertMidilist(m_pszFileList,"tmplist.txt");
	
	//Read for the first time to get the total line, and remove the last line
	InputModelFile.open("tmplist.txt",NULL);
	if(!InputModelFile){
		std::cout<<"failed"<<endl;
		return false;
	}

	while(!InputModelFile.eof()){
		InputModelFile>>MidiFilename>>trackNo>>MetaInfo;
		countLine++;
	}
	InputModelFile.clear();
	InputModelFile.close();
	
	//Read for the second time to get each item
	InputModelFile.open("tmplist.txt",NULL);
	if(!InputModelFile){
		std::cout<<"failed"<<endl;
		return false;
	}
		
	for(i=0;i<countLine;i++){
		InputModelFile>>MidiFilename>>trackNo>>MetaInfo;
		if(MidiFilename!=""){
			MidiMetaInfo myMidiInfo;
			myMidiInfo.MetaInfo=MetaInfo;
			myMidiInfo.MidiFilename=MidiFilename;
			m_FileTrackmap.insert(make_pair(trackNo,myMidiInfo));
		}
	}
	
	InputModelFile.close();
	return true;	
}

string BuildSDHummingModel::GetSongName(string myStr){
	int pos=myStr.find("{");
	return myStr.substr(0,pos);
}

int BuildSDHummingModel::writeSongInfo(char *OutputModelFileName){
	ofstream OUTFILE;
	OUTFILE.open(OutputModelFileName,NULL);
	vector<ModelStru>::iterator VecIter;
	OUTFILE<<m_QBHModel.size()<<endl;
	for(VecIter=m_QBHModel.begin();VecIter!=m_QBHModel.end();++VecIter){
		OUTFILE<<VecIter->MetaInfo<<endl;
	}
	OUTFILE.close();
	return 0;
}

int BuildSDHummingModel::writeExtractedTrack(char *OutputModelFileName)
{
	int i;
	int count_iter=0;
	FILE *OUTFILE=fopen(OutputModelFileName,"wb");
	vector<ModelStru>::iterator VecIter;
	int temp=0;
	int file_count=m_QBHModel.size();
	fwrite(&file_count,sizeof(int),1,OUTFILE);
	int countTrack=0;
	
	for(VecIter=m_QBHModel.begin();VecIter!=m_QBHModel.end();++VecIter){
		fwrite(&VecIter->id,sizeof(int),1,OUTFILE);
		temp=VecIter->notes.size();
		fwrite(&temp,sizeof(int),1,OUTFILE);
		
		for(i=0;i<VecIter->notes.size();i++){
			fwrite(&VecIter->notes[i].Pitch,sizeof(float),1,OUTFILE);	
			fwrite(&VecIter->notes[i].Duration,sizeof(float),1,OUTFILE);	
		}

#ifdef PHRASE_SEGMENTATION
		//ÀÖ¾äÇÐ·ÖÐÅÏ¢
		temp=VecIter->PhraseOffsetVector.size();
		fwrite(&temp,sizeof(int),1,OUTFILE);
		
		for(i=0;i<VecIter->PhraseOffsetVector.size();i++){
			fwrite(&VecIter->PhraseOffsetVector[i],sizeof(int),1,OUTFILE);
		} 
#endif
	}

	fclose(OUTFILE);
	
	return 0;
}

bool BuildSDHummingModel::ExtractTrackNo(int TrackNo,string MetaInfo,char* filename){
	string SName=GetSongName(MetaInfo);
	int k=0;
	int tag=0;
	
	for(k=0;k<m_SongNameVector.size();k++){
		if(SName==m_SongNameVector[k])
			tag=1;
	}

	if(tag==0){
		m_SongNameVector.push_back(SName);
		int i=0,j=0,CountTrackNoteNum=0;
		for(i=0;i<TotalNoteNumber;i++){
			if(pMidiNoteStruct[i].itrackNo==TrackNo){
				m_pExtractedTrack.push_back(pMidiNoteStruct[i]);
				CountTrackNoteNum++;
			}
		}
		
		if(m_pExtractedTrack.size()==0){
			std::cout<<"There is no valid notes in the annotated track, please check!"<<endl;
			return false;
		}
		
		for(i=0;i<CountTrackNoteNum-1;i++){
			m_pExtractedTrack[i].iDurationWithRest=m_pExtractedTrack[i+1].iNoteStartTime-m_pExtractedTrack[i].iNoteStartTime;
		}
				
		ModelStru myQBHModel;
		
		int maxID=0;
		for(i=0;i<m_QBHModel.size();i++){
			if(maxID<m_QBHModel[i].id)
				maxID=m_QBHModel[i].id;
		}
		myQBHModel.id=maxID+1;
		
		for(i=0;i<CountTrackNoteNum;i++){
			NOTE_TEMPLATE Note;
			Note.Pitch=m_pExtractedTrack[i].iNote;
			Note.Duration=m_pExtractedTrack[i].iDurationms/10.0f;
			Note.Duration=(int)((Note.Duration/5)+0.5);
			
			Note.DurationWithRest=m_pExtractedTrack[i].iDurationWithRest/10.0f;
			Note.DurationWithRest=(int)((Note.DurationWithRest/5)+0.5);
			myQBHModel.notes.push_back(Note);
			if(m_pExtractedTrack[i].iPhraseTag==1)
				myQBHModel.PhraseOffsetVector.push_back(i);
		}
		
		myQBHModel.MetaInfo=MetaInfo;
		m_QBHModel.push_back(myQBHModel);
	}
	
	delete[] pMidiNoteStruct;
	m_pExtractedTrack.clear();
	return true;
}

int main(int argc, char* argv[]){
	if (argc != 3){ 
		printf("usage: SDHBuildModel.exe mid_list.txt model_dir\n"); 
		return 0; 
	}

	BuildSDHummingModel myMidi(argv[1],argv[2]);
	
	if(!myMidi.GenFilelist()){
		std::cout<<"ERROR: Could Not Loading Label File!"<<endl;
		return 0;
	}
	
	if(myMidi.Write2Model()!=0){
		std::cout<<"ERROR: Could Not Write New Model!"<<endl;
		return 0;
	}
	return 0;
}