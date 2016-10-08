
#ifndef _VIEWER_SEQUENCE_SET_H_
#define _VIEWER_SEQUENCE_SET_H_

#include "ViewerSequence.h"
#include "SequenceSet.h"

#define KEY_STEP_SELECTED_COMMON_TRACK_SEQUENCE		9	// Tab
#define KEY_STEP_DRAW_SEQUENCE_TYPE					'q'
#define KEY_COLOR_SEGMENTS							'g'
#define KEY_INPUT_SEQUENCE							19	// Ctrl + s

//#define COLOR_SEQUENCE_CONNECTED_COMPONENT_R		255
//#define COLOR_SEQUENCE_CONNECTED_COMPONENT_G		0
//#define COLOR_SEQUENCE_CONNECTED_COMPONENT_B		255

class ViewerSequenceSet : public ViewerSequence
{

public:

	ViewerSequenceSet() : ViewerSequence(), m_pSeqSet(NULL) {}
	virtual void Run(const SequenceSet &seqs, const SequenceIndex &iSeqActive = 0);

protected:

	virtual void Initialize(const SequenceSet &seqs, const SequenceIndex iSeqActive = 0);
	bool PrepareActiveSequence(const SequenceIndex iSeqActive);
	virtual bool PrepareSelectedTrack(const TrackIndex iTrkSelected);
	virtual void DrawAllProjections();
	virtual void DrawSceneView();
	virtual void DrawSceneCameras();
	virtual void DrawSelectedScenePoint();
	virtual void DrawActiveScenePoints();
	virtual void DrawFrameBar();
	virtual bool DragFrameBar(const CVD::ImageRef &from, const CVD::ImageRef &to);
	virtual void ComputeDepths(std::vector<float> &depths);

	virtual bool OnKeyDown(const int key);
	virtual bool OnMouseDoubleClicked(const CVD::ImageRef &where, const int button);

protected:

	const SequenceSet *m_pSeqSet;
	SequenceIndex m_iSeqActive;
	TrackIndex m_iTrkCmnSelected;
	FrameIndexList m_iFrmsActive;
	enum DrawSequenceType { DRAW_ACTIVE_SEQUENCE, DRAW_REGISTERED_SEQUENCES_INACTIVE_GRAY, DRAW_ALL_SEQUENCES_INACTIVE_GRAY, DRAW_ALL_SEQUENCES } m_drawSeqType;
	bool m_clrSegs, m_graySeqPath;
	std::vector<SegmentIndexList> m_mapSeqFrmToSeg;
	std::vector<std::vector<uint> > m_mapSeqFrmToCatFrm;
	SequenceFrameIndexList m_mapCatFrmToSeqFrm;
	std::vector<SequenceFramePairIndex> m_mapSegToSeqFrm;
	std::vector<CVD::Rgb<ubyte> > m_pathClrsSeq, m_pathClrsSeg;
	SequenceIndexList m_iSeqsCC;

};

#endif