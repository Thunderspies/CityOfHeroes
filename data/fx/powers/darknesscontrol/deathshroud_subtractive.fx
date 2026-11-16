#########################################################
## Dark Armor - Death Shroud (Subtractive)
#########################################################

FxInfo

## SOUNDS #######################################################

Condition

	On	Time
	Time	28

	Event
		Type	Local
		At	Origin
		Sound darkarmor2 60 60 .88
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	Root
		Sound forcefield5_loop 60 60 .44
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	700
	End

	Event
		Type	Local
		At	Root
		Sound forcefield5_loop 60 60 .44
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	700
	End
End

## POWER FX #######################################################

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	FootR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		Type	Local
		At	FootL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown_Subtractive.part
	End
End    

#########################################################

Condition
	On	Time
	Time	75

	Event
		EName	9a5
		Type	Local
		At	Root
		part	:DarkArmorDamageRing.Part
		Part1	:DarkArmorRing.Part
		part2	:DarkBlastHitTendrilOut_Subtractive.part
		part3	:SmokeOut.part
		part4	:Smoke2Out.part
		#part	:SmokeOutFlat.part
		#part	:Smoke2OutFlat.part
		Sound rage 60 60 .75
	End
End

#########################################################

#Condition
#	On	Cycle
#	Time	60
#
#	Event
#		Type	Local
#		At	Root
#		Bhvr	:DarkRing.bhvr
#		Splat	Generic_Ring.tga
#		LifeSpan	60
##		part	:DarkArmorDamageRing.Part
##		Part1	:DarkArmorRing.Part
#	End
#End
#
#Condition
#	On	Cycle
#	Time	60
#
#	Event
#		Type	Local
#		At	Root
#		Bhvr	:DarkRingRed.bhvr
#		Splat	Generic_Ring.tga
#		LifeSpan	60
#	End
#End

#########################################################

End