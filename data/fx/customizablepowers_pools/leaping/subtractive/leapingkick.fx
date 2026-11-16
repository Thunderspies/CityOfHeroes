#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	Posit
		At	FootR
		Sound leapingkick3 60 60 .8
		part5	:LeapingKickMist.part
		Lifespan 75
	End

	Event
		Type	Posit
		At	FootR
		part3	:LeapingKickSteam.part
		part2	:LeapingKickCloudSteam.part
		part4	:LeapingKickCloudSteam2.part
		Lifespan 30
	End

	Event
		Type	Posit
		At	FootL
		part5	:LeapingKickMist.part
		Lifespan 75
	End

	Event
		Type	Posit
		At	FootL
		part3	:LeapingKickSteam.part
		part4	:LeapingKickCloudSteam2.part
		part2	:LeapingKickCloudSteam.part
		Lifespan 30
	End

	Event
		Type	Posit
		At	WepL
		part5	:LeapingKickMist.part
		Lifespan 75
	End

	Event
		Type	Posit
		At	WepL
		part2	:LeapingKickCloudSteam.part
		part3	:LeapingKickSteam.part
		part4	:LeapingKickCloudSteam2.part
		Lifespan 30
	End

	Event
		Type	Posit
		At	WepR
		part5	:LeapingKickMist.part
		Lifespan 75
	End

	Event
		Type	Posit
		At	WepR
		part2	:LeapingKickCloudSteam.part
		part3	:LeapingKickSteam.part
		part4	:LeapingKickCloudSteam2.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	74

	Event
		Type	Posit
		At	Root
		Geom	Torrent
	End

	Event
		Type	Posit
		At	FootR
		part1	:LeapingKickRingsUpBack.part
		LifeSpan	10
	End
End

Condition
	On	Time
	Time	26

	Event
		Type	Posit
		At	Root
		Geom	Torrent
	End

	Event
		Type	Posit
		At	ALT
		Altpiv	1
		part1	:LeapingKickRingsUp.part
		LifeSpan	10
	End

	Event
		Type	Posit
		At	WepL
		part1	:LeapingKickGlow.part
		part2	:LeapingKickBurst.part
		Lifespan 64
	End

	Event
		Type	Posit
		At	WepR
		part1	:LeapingKickGlow.part
		part2	:LeapingKickBurst.part
		Lifespan 64
	End
End

Condition
	On	Time
	Time	75

	Event
		Type	Posit
		At	WepR
		part2	:LeapingKickCloudSteam.part
		#part1	:LeapingCircleBase2.part
		part3	:LeapingKickSteam.part
		Lifespan 15
	End

	Event
		Type	Posit
		At	WepL
		part2	:LeapingKickCloudSteam.part
		#part1	:LeapingCircleBase2.part
		part3	:LeapingKickSteam.part
		Lifespan 15
	End

	Event
		Type	Posit
		At	FootL
		part2	:LeapingKickCloudSteam.part
		#part1	:LeapingCircleBase2.part
		part3	:LeapingKickSteam.part
		Lifespan 15
	End

	Event
		Type	Posit
		At	FootR
		part2	:LeapingKickCloudSteam.part
		#part1	:LeapingCircleBase2.part
		part3	:LeapingKickSteam.part
		Lifespan 15
	End
End

#############################################################

End