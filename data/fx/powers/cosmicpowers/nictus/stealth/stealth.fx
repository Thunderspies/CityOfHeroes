#########################################################
## Umbral Aura - Shadow Cloak
#########################################################

FxInfo

## SOUND FX #######################################################

Condition

	On	Time
	Time	20
	
	Event
		Type	Local 
		At	Eyes
		Sound TransBurst 70 70 .55
	End
End

Condition
	On	Time
	Time	60
	
	Event
		EName	2
		Type	Local
		At	FootR
		Sound blue_loop 60 60 .4
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 370
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		Sound intel_loop 60 60 .33
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 370
	End
End

## EYES FX #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName 	Eyes
#		Type	Local 
#		At	Root
#		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyesContinuingFX.fx
#	End
#End

#########################################################

Condition
	On	Time
	Time	0 #60

	Event
		EName	9
		Type	Local
		At	Head
		##part1	:Smokex.part
		part2	:Smoke2x.part
		##Part	:PurpleTendrilsLocal.part
		##Part	:BlueTendrilsLocal.part
		Part	:PurpleTendrils.part
		##part	:Smokes.part
		part	:Smoke2s.part
		Part	:BlueTendrils.part
	End

#	Event
#		EName	9
#		Type	Local
#		At	Chest
#		##part1	:Smokex.part
#		part2	:Smoke2x.part
#		##Part	:PurpleTendrilsLocal.part
#		##Part	:BlueTendrilsLocal.part
#		Part	:PurpleTendrils.part
#		##part	:Smokes.part
#		part	:Smoke2s.part
#		Part	:BlueTendrils.part
#	End

#	Event
#		EName	10
#		Type	Local
#		At	UArmL
#		##part1	:Smokex.part
#		part2	:Smoke2x.part
#		##Part	:PurpleTendrilsLocal.part
#		##Part	:BlueTendrilsLocal.part
#		Part	:PurpleTendrils.part
#		##part	:Smokes.part
#		part	:Smoke2s.part
#		Part	:BlueTendrils.part
#		#PMagnet LArmL
#	End
#
#	Event
#		EName	13
#		Type	Local
#		At	UArmR
#		##part1	:Smokex.part
#		part2	:Smoke2x.part
#		##Part	:PurpleTendrilsLocal.part
#		##Part	:BlueTendrilsLocal.part
#		Part	:PurpleTendrils.part
#		##part	:Smokes.part
#		part	:Smoke2s.part
#		Part	:BlueTendrils.part
#		#PMagnet LArmR
#	End

	Event
		EName	11
		Type	Local
		At	LArmL
		##part1	:Smokex.part
		part2	:Smoke2x.part
		##Part	:PurpleTendrilsLocal.part
		##Part	:BlueTendrilsLocal.part
		Part	:PurpleTendrils.part
		##part	:Smokes.part
		part	:Smoke2s.part
		Part	:BlueTendrils.part
		#PMagnet WepL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		##part1	:Smokex.part
		part2	:Smoke2x.part
		##Part	:PurpleTendrilsLocal.part
		##Part	:BlueTendrilsLocal.part
		Part	:PurpleTendrils.part
		##part	:Smokes.part
		part	:Smoke2s.part
		Part	:BlueTendrils.part
		#PMagnet WepR
	End

	Event
		EName	3
		Type	Local
		At	Hips
		##part1	:Smokex.part
		part2	:Smoke2x.part
		##Part	:PurpleTendrilsLocal.part
		##Part	:BlueTendrilsLocal.part
		Part	:PurpleTendrils.part
		##part	:Smokes.part
		part	:Smoke2s.part
		Part	:BlueTendrils.part
		#PMagnet LLEGL
	End

#	Event
#		EName	3
#		Type	Local
#		At	ULEGL
#		##part1	:Smokex.part
#		part2	:Smoke2x.part
#		##Part	:PurpleTendrilsLocal.part
#		##Part	:BlueTendrilsLocal.part
#		Part	:PurpleTendrils.part
#		##part	:Smokes.part
#		part	:Smoke2s.part
#		Part	:BlueTendrils.part
#		#PMagnet LLEGL
#	End
#
#	Event
#		EName	6
#		Type	Local
#		At	ULEGR
#		##part1	:Smokex.part
#		part2	:Smoke2x.part
#		##Part	:PurpleTendrilsLocal.part
#		##Part	:BlueTendrilsLocal.part
#		Part	:PurpleTendrils.part
#		##part	:Smokes.part
#		part	:Smoke2s.part
#		Part	:BlueTendrils.part
#		#PMagnet LlegR
#	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		##part1	:Smokex.part
		part2	:Smoke2x.part
		##Part	:PurpleTendrilsLocal.part
		##Part	:BlueTendrilsLocal.part
		Part	:PurpleTendrils.part
		##part	:Smokes.part
		part	:Smoke2s.part
		Part	:BlueTendrils.part
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		##part1	:Smokex.part
		part2	:Smoke2x.part
		##Part	:PurpleTendrilsLocal.part
		##Part	:BlueTendrilsLocal.part
		Part	:PurpleTendrils.part
		##part	:Smokes.part
		part	:Smoke2s.part
		Part	:BlueTendrils.part
		#PMagnet FootR
	End
	
	Event
		EName	5		
		Type	Local
		At	FootL
		##part1	:Smokex.part
		part2	:Smoke2x.part
		##Part	:PurpleTendrilsLocal.part
		##Part	:BlueTendrilsLocal.part
		Part	:PurpleTendrils.part
		##part	:Smokes.part
		part	:Smoke2s.part
		Part	:BlueTendrils.part
	End
	
	Event
		EName	2
		Type	Local
		At	FootR
		##part1	:Smokex.part
		part2	:Smoke2x.part
		##Part	:PurpleTendrilsLocal.part
		##Part	:BlueTendrilsLocal.part
		Part	:PurpleTendrils.part
		##part	:Smokes.part
		part	:Smoke2s.part
		Part	:BlueTendrils.part
		#PMagnet LLEGR
	End
End     

#########################################################

End