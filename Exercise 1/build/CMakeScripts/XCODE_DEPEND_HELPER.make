# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.MUSI6106Exec.Debug:
PostBuild.AudioFileIO.Debug: /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/debug/MUSI6106Exec
PostBuild.sndlib.Debug: /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/debug/MUSI6106Exec
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/debug/MUSI6106Exec:\
	/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/debug/libAudioFileIO.a\
	/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/debug/libsndlib.a
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/debug/MUSI6106Exec


PostBuild.AudioFileIO.Debug:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/debug/libAudioFileIO.a:
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/debug/libAudioFileIO.a


PostBuild.sndlib.Debug:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/debug/libsndlib.a:
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/debug/libsndlib.a


PostBuild.MUSI6106Exec.Release:
PostBuild.AudioFileIO.Release: /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/release/MUSI6106Exec
PostBuild.sndlib.Release: /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/release/MUSI6106Exec
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/release/MUSI6106Exec:\
	/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/release/libAudioFileIO.a\
	/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/release/libsndlib.a
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/release/MUSI6106Exec


PostBuild.AudioFileIO.Release:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/release/libAudioFileIO.a:
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/release/libAudioFileIO.a


PostBuild.sndlib.Release:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/release/libsndlib.a:
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/release/libsndlib.a


PostBuild.MUSI6106Exec.MinSizeRel:
PostBuild.AudioFileIO.MinSizeRel: /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/MinSizeRel/MUSI6106Exec
PostBuild.sndlib.MinSizeRel: /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/MinSizeRel/MUSI6106Exec
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/MinSizeRel/MUSI6106Exec:\
	/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/MinSizeRel/libAudioFileIO.a\
	/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/MinSizeRel/libsndlib.a
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/MinSizeRel/MUSI6106Exec


PostBuild.AudioFileIO.MinSizeRel:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/MinSizeRel/libAudioFileIO.a:
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/MinSizeRel/libAudioFileIO.a


PostBuild.sndlib.MinSizeRel:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/MinSizeRel/libsndlib.a:
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/MinSizeRel/libsndlib.a


PostBuild.MUSI6106Exec.RelWithDebInfo:
PostBuild.AudioFileIO.RelWithDebInfo: /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/relwithdebinfo/MUSI6106Exec
PostBuild.sndlib.RelWithDebInfo: /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/relwithdebinfo/MUSI6106Exec
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/relwithdebinfo/MUSI6106Exec:\
	/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/relwithdebinfo/libAudioFileIO.a\
	/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/relwithdebinfo/libsndlib.a
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/bin/relwithdebinfo/MUSI6106Exec


PostBuild.AudioFileIO.RelWithDebInfo:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/relwithdebinfo/libAudioFileIO.a:
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/relwithdebinfo/libAudioFileIO.a


PostBuild.sndlib.RelWithDebInfo:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/relwithdebinfo/libsndlib.a:
	/bin/rm -f /Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/relwithdebinfo/libsndlib.a




# For each target create a dummy ruleso the target does not have to exist
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/MinSizeRel/libAudioFileIO.a:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/MinSizeRel/libsndlib.a:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/debug/libAudioFileIO.a:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/debug/libsndlib.a:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/release/libAudioFileIO.a:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/release/libsndlib.a:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/relwithdebinfo/libAudioFileIO.a:
/Users/zhaoyan/workstuff/GatechStuff/6106/AudioSoftwareEngineeringHW/Exercise\ 1/lib/relwithdebinfo/libsndlib.a:
