/*****************************************************************************

	bioviewer

    Copyright (C) 2002  Lucas Walter

  	lucasw@u.washington.edu
	http://students.washington.edu/lucasw

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

******************************************************************************/

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "misc.hpp"
#include "bvh.hpp"

using namespace std;

bvhPart::bvhPart()
{
	/* objPart = NULL; */
}

bvh::bvh(string bvhFile)
{
	verbose = true;

	init(bvhFile);

	return;
}

void bvh::process(string line)
{
	//bool switched = false;
	if (line == "OFFSET") {
		//	if (theMode != Site) {
				vertIndex = 0;
				theMode = OFFSET;
		//	}
	} else if (line == "ROOT") {
		root = new bvhPart;
		root->parent = NULL;
		current = root;

		theMode = ROOT;
	} else if (line == "{") {

	} else if (line == "}") {
	//	if (theMode == Site) { theMode = NONE; }
	//	else
			if (current != root) {
				current = current->parent;
		       		theMode = NONE;
			}
	} else if (line == "JOINT") {
		bvhPart *temp = new bvhPart;
		temp->parent = current;
		current->child.push_back(temp);
		current = temp;
		theMode = JOINT;
	} else if (line == "CHANNELS") {
		theMode = CHANNELS;
	} else if (line == "End") {
                theMode = End;
	} else if (line == "Site") {
		bvhPart *temp = new bvhPart;
		temp->parent = current;
		current->child.push_back(temp);
		current = temp;
		theMode = NONE; //Site;
	} else if (line == "MOTION") {
		theMode = MOTION;
	} else if (line == "Frames:") {
		theMode = Frames;
	} else if (line == "Frame") {
		theMode = Frame;
	} else if (line == "Time:") {
		theMode = Time;
	} else {

		switch (theMode) {
			case (ROOT):
				root->name = line;

				theMode = NONE;
			break;

			case (JOINT):
				current->name = line;

				/*
				if (line == "LeftKnee") {
					try { current->objPart = new rigid("obj/leftforeleg.obj"); }
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightKnee") {
					try { current->objPart = new rigid("obj/rightforeleg.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftHip") {
					try { current->objPart = new rigid("obj/leftthigh.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightHip") {
					try { current->objPart = new rigid("obj/rightthigh.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftAnkle") {
					try { current->objPart = new rigid("obj/leftfoot.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightAnkle") {
					try { current->objPart = new rigid("obj/rightfoot.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "Chest") {
					//current->objPart = new objloader("obj/torso.obj",0,subdir);
				} else if (line == "Chest2") {
					try { current->objPart = new rigid("obj/torso.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftShoulder") {
					try { current->objPart = new rigid("obj/leftshoulder.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightShoulder") {
					try { current->objPart = new rigid("obj/rightshoulder.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftElbow") {
					try { current->objPart = new rigid("obj/leftforearm.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightElbow") {
					try { current->objPart = new rigid("obj/rightforearm.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftWrist") {
					try { current->objPart = new rigid("obj/lefthand.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightWrist") {
					try { current->objPart = new rigid("obj/righthand.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "Neck") {
					try { current->objPart = new rigid("obj/neck.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "Head") {
					try { current->objPart = new rigid("obj/head.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				}
				*/

				//	cout << line << "\n";
				theMode = NONE;
			break;

			case (OFFSET):
				current->offset.vertex[vertIndex] = atof(line.c_str());
				vertIndex++;
				if (vertIndex >2) {
					vertIndex = 0;
					theMode = NONE;
				}
			break;

			case (CHANNELS):
				// assume channelsNum == 0 in the .bvh .is impossible
				if (channelsNum == 0) {
					channelsNum = atoi(line.c_str());
				} else if (line == "Xposition") {
					current->channels.push_back(bvhPart::Xpos);
				} else if (line == "Yposition") {
					current->channels.push_back(bvhPart::Ypos);
				} else if (line == "Zposition") {
					current->channels.push_back(bvhPart::Zpos);
				} else if (line == "Zrotation") {
					current->channels.push_back(bvhPart::Zrot);
				} else if (line == "Yrotation") {
					current->channels.push_back(bvhPart::Yrot);
				} else if (line == "Xrotation") {
					current->channels.push_back(bvhPart::Xrot);
				}

				// if there are additional channel types in error, they'll be ignored(?)
				if (current->channels.size() == channelsNum) {
					theMode = NONE;
					channelsNum = 0;
				}
				break;

			case (Frames):
				framesNum = atoi(line.c_str());
				theMode = NONE;

				break;

			case (Frame):
				break;

			case (Time):
				frameTime = atof(line.c_str());
				theMode = MOTIONDATA;
				current = root;
				recurs(root);

				break;

			case (MOTIONDATA):

				data++;
				switch (bvhPartsLinear[partIndex]->channels[channelIndex]) {
					case (bvhPart::Xpos):
						tempMotion.Translate(atof(line.c_str()),0,0);
						channelIndex++;
						break;
					case (bvhPart::Ypos):
						tempMotion.Translate(0,atof(line.c_str()),0);
						channelIndex++;
						break;
					case (bvhPart::Zpos):
						tempMotion.Translate(0,0,atof(line.c_str()));
						channelIndex++;
						break;
					case (bvhPart::Zrot):
						// it seems like some limbs need a negative, and out
						// limbs don't
						tempMotionZ.RotateZ((float)-DEG_TO_RAD(atof(line.c_str())));
						channelIndex++;
						break;
					case (bvhPart::Yrot):
						//if (partIndex == 3) cout << atof(line.c_str()) << "\n";
						tempMotionY.RotateY((float)-DEG_TO_RAD(atof(line.c_str())));
						//tempMotion.print();
						channelIndex++;
						break;
					case (bvhPart::Xrot):
						//if (partIndex == 3) cout << atof(line.c_str()) << "\n";
						tempMotionX.RotateX((float)-DEG_TO_RAD(atof(line.c_str())));
						channelIndex++;
						break;
				}

				if (channelIndex >= bvhPartsLinear[partIndex]->channels.size()) {
					// store tempMotion and move to next part
					tempMotion = tempMotion * (tempMotionZ *tempMotionX * tempMotionY );// * tempMotionY);
					bvhPartsLinear[partIndex]->motion.push_back(tempMotion);
					//tempMotion.print();

					tempMotion.LoadIdentity();
					tempMotionX.LoadIdentity();
					tempMotionY.LoadIdentity();
					tempMotionZ.LoadIdentity();

	/*				tempMotion.matrix[10] = -1.0f;
					tempMotionX.matrix[10] = -1.0f;
					tempMotionY.matrix[10] = -1.0f;
					tempMotionZ.matrix[10] = -1.0f;
	*/

					channelIndex = 0;
					partIndex++;
				}
				if (partIndex >= bvhPartsLinear.size()) {
					// this should be the end of one line of motion data
					partIndex = 0;
				}

			case (NONE):
			case (End):
			case (Site):
			case (MOTION):
				break;

		}
	}
	/*
	if (mode == 1) {
		v_ind++;
		if (v_ind == 2) v_ind = 0;
	}*/
}

void bvh::recurs(bvhPart* some)
{
	matrix16f motion0;
	motion0.LoadIdentity();
//	motion0.matrix[10] = -1.0f;

	motion0.Translate(some->offset.vertex[0],some->offset.vertex[1],some->offset.vertex[2]);
	some->motion.push_back(motion0);
	if (some->child.size() != 0) bvhPartsLinear.push_back(some);
//	cout << some->name << some->offset.vertex[0] << " " <<
//				some->offset.vertex[1] << " " <<
//				some->offset.vertex[2] << "\n";
	unsigned i;
	for (i = 0; i < some->child.size(); i++)
		recurs(some->child[i]);

}

void bvh::init(string bvhFile)
{
	data = 0;
	partIndex = 0;
	channelIndex = 0;
	//string bvhFile = "bvh/Take 006.bvh";

	tempMotion.LoadIdentity();
	tempMotionX.LoadIdentity();
	tempMotionY.LoadIdentity();
	tempMotionZ.LoadIdentity();

/*	tempMotion.matrix[10] = -1.0f;
	tempMotionX.matrix[10] = -1.0f;
	tempMotionY.matrix[10] = -1.0f;
	tempMotionZ.matrix[10] = -1.0f;
*/

	ifstream bvhStream(bvhFile.c_str());
	if (!bvhStream) {
		cout << "File \"" << bvhFile << "\" not found.\n";
		return;
	}
	if (verbose) cout << "File \"" << bvhFile << "\" found.\n";
	istream_iterator<string> bvhIt(bvhStream);
	istream_iterator<string> sentinel;

	vector<string> lines(bvhIt,sentinel);


	//	for_each(lines.begin(),lines.end(),&bvh::process);

	unsigned i;
	for (i =0; i< lines.size(); i++) {
		process(lines[i]);
	}

	bvhStream.close();

	framesNum = bvhPartsLinear[0]->motion.size();

	//if (verbose) cout << "\n\n";
	if (verbose) cout << "framesNum = " << framesNum << "\nframetime = " << frameTime << "\n";
	if (verbose) cout << data/(framesNum*3)-1 << " bodyparts\n";

	//for (i =0; i< root->motion.size(); i++)
	//	root->child[0]->child[0]->motion[i].print();
/*
	for (i = 0; i < bvhPartsLinear[1]->motion.size(); i++) {
		bvhPartsLinear[1]->motion[i].print();
	}
*/
	//return;
}

