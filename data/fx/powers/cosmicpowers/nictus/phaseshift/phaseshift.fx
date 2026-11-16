########################################################
## Umbral Aura - Nebulous Form
########################################################

FxInfo

## SOUND FX ######################################################

Condition
	On	Time
	Time	50
	
	Event
		Type	Local 
		At	Eyes
		Sound TransBurst 60 60 .7
	End
End

Condition
	On	Time
	Time	60

	Event
		EName	9
		Type	Local
		At	Chest

		Sound intel2_loop 60 60 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 330
	End

	Event
		EName	9
		Type	Local
		At	Chest

		Sound green_loop 60 60 .22
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 400
	End
End

## EYE FX ######################################################

##Condition
##	On	Time
##	Time	0
##
##	Event
##		EName 	Eyes
##		Type	Local 
##		At	Root
##		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyesContinuingFX.fx
##	End
##End

## AURA FX ######################################################

Condition
	On	Time
	Time	0 #60

	Event
		EName	9
		Type	Local
		At	Head
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

	Event
		EName	3
		Type	Local
		At	Hips
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

##	Event
##		EName	3
##		Type	Local
##		At	ULEGL
##		part1	:Smokex.part
##		part2	:Smoke2x.part
##		Part	:PurpleTendrilsLocal.part
##		Part	:BlueTendrilsLocal.part		
##		Part	:PurpleTendrils.part
##		part	:Smokes.part
##		part	:Smoke2s.part
##		Part	:BlueTendrils.part
##	End
##
##	Event
##		EName	6
##		Type	Local
##		At	ULEGR
##		part1	:Smokex.part
##		part2	:Smoke2x.part
##		Part	:PurpleTendrilsLocal.part
##		Part	:BlueTendrilsLocal.part
##		Part	:PurpleTendrils.part
##		part	:Smokes.part
##		part	:Smoke2s.part
##		Part	:BlueTendrils.part
##	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End

	Event
		EName	2
		Type	Local
		At	FootR
		Part	:Smoke2x.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
	End
End     

########################################################

End