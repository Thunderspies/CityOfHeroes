#########################################################
##	Fireweapon
########################################################
FxInfo

Condition
	On 	Time
	Time 	0
########################################################
#Fire&Sparks
########################################################

	
	Event
		EName 	fire1
		Type	Local
		At	Col_R
		
		Part1	:MinionFlame.part
		Part2	:MinionSpark.part
		Sound torch1_loop 70 70 .25
		
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Col_L
		
		Part1	:MinionFlame.part
		Part2	:MinionSpark.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	Back
		
		Part1	:MinionFlameBack.part
		Part2	:MinionSpark.part
		
	End
	

	Event
		EName 	fire2
		Type	Local
		At	UarmR
		
		Part1	:MinionFlameArm.part
		Part2	:MinionSpark.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:MinionFlameArm.part
		Part2	:MinionSpark.part
		
	End


########################################################
#Eyes
########################################################	
	
	Event
		EName 	hookup
		Type	Local
		At	Head
		Geom	Minion_Eyey

		
	End

	Event
		EName 	headhook1
		Type	Local
		At	head
		##altpiv	1
		part1	:DaemonEyes1x.part
		part2	:DaemonPupilx.part
		Part3	:BehemothEyesMistLeftx.part
		bhvr	:eyes_left.bhvr
	End

	Event
		EName 	headhook2
		Type	Local
		At	head
		##altpiv	2
		part1	:DaemonEyes1x.part
		part2	:DaemonPupilx.part
		Part3	:BehemothEyesMistRightx.part
		bhvr	:eyes_right.bhvr
	End

	

End

########################################################
#Snort Holes
########################################################

Condition
	On 	cycle
	Time	100
	Chance	1

	Event
		EName 	Prime
		Type	Local 
		At	hookup
		altpiv	4
		part1	:SnortSmokeRight.part
		Sound behemoth_noseblast 30 30 .25
		
		lifespan	25
	End

	Event
		EName 	headhook
		Type	Local
		At	hookup
		altpiv	5
		part1	:SnortSmokeLeft.part
		
		lifespan	25
	End

End


End
##################################

