// ENFT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FeatureTracker.h"
#include "TrackMatcher.h"
#include "ViewerSequenceSet.h"

#include "Z:/Graphics/glut/lib/link.h"
#include "Z:/Graphics/glew/lib/link.h"
#include "Z:/CVD/lib/link.h"
#include "Z:/Math/Numerical/clapack-3.2.1-CMAKE/lib/link.h"
#include "Z:/Math/Numerical/levmar-2.6/lib/link.h"

typedef struct Video
{
	const char *imgFileName;
	int start, step, end;
} Video;

void Run(const std::vector<Video> &videos, const char *paramDir, const char *outputFileName, const bool view = false)
{
	SequenceSet Vs;
	Vs.SetDirectory("");
	Vs.SetIntrinsicType(Sequence::INTRINSIC_CONSTANT);

	const int nVideos = int(videos.size());
	Vs.CreateSequences(SequenceIndex(nVideos));

	FeatureTracker ftrTracker;
	TrackMatcher trkMatcher1, trkMatcher2;
	for(int iVideo = 0; iVideo < nVideos; ++iVideo)
	{
		const Video &video = videos[iVideo];
		const std::string dir = IO::ExtractFileDirectory(video.imgFileName);
		const std::string imgFileName = IO::RemoveFileDirectory(video.imgFileName);
		const int iStart = video.start, iStep = video.step, iEnd = video.end;
		Sequence &V = Vs[iVideo];
		V.SetTag(dir, imgFileName, iStart, iStep, iEnd);
		V.SetIntrinsicType(Sequence::INTRINSIC_CONSTANT);
		V.LoadCalibrationFile(NULL);

		if(iVideo == 0)
		{
			ftrTracker.Initialize(V, std::string(paramDir) + "param_ftr_tracking.txt");
			trkMatcher1.Initialize(V, std::string(paramDir) + "param_trk_matching.txt");
		}

		ftrTracker.Run(V);
		trkMatcher1.Run(V);
	}

	if(nVideos > 1)
	{
		int iVideo1, iVideo2;
		SequenceIndexPairList iVideoPairs;
		for(iVideo1 = 0; iVideo1 < nVideos; ++iVideo1)
		for(iVideo2 = iVideo1 + 1; iVideo2 < nVideos; ++iVideo2)
			iVideoPairs.push_back(SequenceIndexPair(SequenceIndex(iVideo1), SequenceIndex(iVideo2)));
		trkMatcher2.Initialize(Vs, std::string(paramDir) + "param_trk_matching.txt");
		trkMatcher2.Run(Vs, iVideoPairs);
	}

	Vs.SaveTracks(outputFileName);

	if(view)
	{
		if(nVideos > 1)
		{
			ViewerSequenceSet viewer;
			viewer.Run(Vs);
		}
		else
		{
			ViewerSequence viewer;
			viewer.Run(Vs[0]);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Video> videos(2);
	videos[0].imgFileName = "E:\\Data\\SfM\\960x540\\rect00000.jpg";
	videos[0].start = 0;
	videos[0].step = 1;
	videos[0].end = 150;
	videos[1].imgFileName = "E:\\Data\\SfM\\960x540\\rect00000.jpg";
	videos[1].start = 150;
	videos[1].step = 1;
	videos[1].end = 258;
	Run(videos, ".\\param\\", "result.txt");
	return 0;
}

