#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Glow7_loop 80 80 .5
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End
End

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		ENAME	ChestGlow
#		Type	Local
#		At	Hips
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	Neck
#	End
#
#	Event
#		ENAME	HeadGlow
#		Type	Local
#		At	Head
#		#part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	Chest
#	End
#
#	Event
#		ENAME	RightUarmGlow
#		Type	Local
#		At	SpadR
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	LarmR
#	End
#
#	Event
#		ENAME	RightLarmGlow
#		Type	Local
#		At	WepR
#		part1	Powers\SuperReflexes\SRBodyStreaks01.part
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	LarmR
#	End
#
#	Event
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	SpadL
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	LarmL
#	End
#
#	Event
#		ENAME	LeftLarmGlow
#		Type	Local
#		At	WepL
#		part1	Powers\SuperReflexes\SRBodyStreaks01.part
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	LarmL
#	End
#
#	Event
#		ENAME	LeftULegGlow
#		Type	Local
#		At	LlegL
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	Ulegl
#	End
#
#	Event
#		ENAME	LeftLLegGlow
#		Type	Local
#		At	FootL
#		part1	Powers\SuperReflexes\SRBodyStreaks01.part
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	Llegl
#	End
#
#	Event
#		ENAME	RightULegGlow
#		Type	Local
#		At	LlegR
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	UlegR
#	End
#
#	Event
#		ENAME	RightLLegGlow
#		Type	Local
#		At	FootR
#		part1	Powers\SuperReflexes\SRBodyStreaks01.part
#		part1	Powers\SuperReflexes\SRBodyGlowLines01.part
#		part1	Powers\SuperReflexes\SRBodyGlows01.part
#		POther	LlegR
#	End
#End

#############################################################

End