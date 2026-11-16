#############################################################
## DullPain.fx
#############################################################

FxInfo

#####################################################################################
##Flens Flares
#####################################################

Condition
	On 	cycle
	Time 	20
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LarmL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

		LifeSpan	25
	End

End

Condition
	On 	cycle
	Time 	19
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LarmR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		LifeSpan	25
	End

End

Condition
	On 	cycle
	Time 	21
	Chance	.2

	Event
		EName	3
		Type	Local
		At	UarmL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		LifeSpan	25
	End

End


Condition
	On 	cycle
	Time 	22
	Chance	.2

	Event
		EName	3
		Type	Local
		At	UarmR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		LifeSpan	25
	End


End

Condition
	On 	cycle
	Time 	17
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		LifeSpan	25
	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		LifeSpan	25
	End


End

#####################################################################################


Condition
	On	Time
	Time	58

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part1 	:Ring03.part

	End

End

Condition
	On	Time
	Time	5

	Event
		Ename	BoostMist
		Type	Local
		At	Hips

		Part2 	:GreenMist.part
		Sound regen8c 60 60 .76
		LifeSpan	60
	End

End

Condition
	On	Time
	Time	0

	Event
		EName	3
		Type	Local
		At	UlegL
		part1	:RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	3
		Type	Local
		At	UlegR
		part1	:RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:RegenBodyGlows2.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	:RegenBodyGlows2.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	FootL
		part1	:RegenBodyGlows3.part
		PMagnet	LLEGL

	End

	Event
		EName	7
		Type	Local
		At	FootR
		part1	:RegenBodyGlows3.part
		PMagnet	LLEGR

	End

####################################################################################
##Original
##RegenBodyGlows2.part
#####################################################################################

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:RegenBodyGlows.part
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:RegenBodyGlows.part


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:RegenBodyGlows.part

		PMagnet UlegL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:RegenBodyGlows.part
		PMagnet LlegL
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:RegenBodyGlows.part


	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:RegenBodyGlows.part

		PMagnet UlegL
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:RegenBodyGlows.part

	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:RegenBodyGlows.part
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:RegenBodyGlows.part
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:RegenBodyGlows.part

	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:RegenBodyGlows.part
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:RegenBodyGlows.part
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:RegenBodyGlows.part

	End

End

###################################################################################

End

