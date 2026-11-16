#########################################################
## Dark Armor - Oppressive Gloom
#########################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	9a5
		Type	Local
		At	root
		Part1	:DarkArmorRing.Part
		Sound darkarmor2 60 60 .75
	End
End

#########################################################

#Condition
#	On Cycle
#	Time 120
#	Event
#		EName 	Prime
#		#At	Origin
#		Type	SetLight
#		BHVR	GENERIC\LowGlow\Darkpulseclamp.bhvr
#		LifeSpan 120
#	End
#
#End

Condition
	On	Time
	Time	50

	Event
		EName	RINGS
		Type	Local
		At	chest
		part	:RedHitMist.part
		part1	:RegenerationRinga.part
	End
End

## SPLATS #######################################################

##Condition
##
##	Event
##		Type	Local
##		At	root
##		Bhvr	:DarkRingQuicker.bhvr
##		Splat	Generic_Ring.tga
##		LifeSpan	60
##	End
##End
##
##Condition
##	On	Cycle
##	Time	50
##
##	Event
##		Type	Local
##		At	root
##		Bhvr	:DarkRingQuicker.bhvr
##		Splat	Generic_Ring.tga
##		LifeSpan	60
##	End
##End

#########################################################

Condition
	On	Time
	Time	60

	Event
		EName	2
		Type	Local
		At	FootR
		Sound miasma_loop 60 60 .6
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	400
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		bhvr	behaviors/soundFade3.bhvr
		Sound glow3_loop 60 60 .4
		Flags	PowerLoopingSound
		Lifespan	400
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Sound gloomy_downer 60 60 .79
	End

	Event
		EName	5
		Type	Local
		At	FootL
		Sound rage 100 100 1.0
	End
End


#########################################################

End