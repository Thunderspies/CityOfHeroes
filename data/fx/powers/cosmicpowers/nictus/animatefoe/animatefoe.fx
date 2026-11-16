#########################################################
## Umbral Blast - Animate Foe
#########################################################

FxInfo

LifeSpan	200

## SOUND FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName	Sound1
		Type	start
		At	T_Root
		Sound WarShadeAnimateFoe 90 90 .83
	End
End

Condition
	On	Time
	Time	10
		
	Event
		EName	Sound2
		Type	local
		At	T_Root
		Sound flameburst_loop 60 60 .74
		Sound fireballfly 60 60 .3
	End
End

## EYE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Eyes
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyes.fx
	End
End

## HANDS ######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepL
		part	:DroneHandGlows.part
		part	:DroneHandGlowsPurple.part
		part	:DroneHandGlowsBlue.part
		part	:Sparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	75
	End
	
	Event
		Type	Local
		At	WepR
		part	:DroneHandGlows.part
		part	:DroneHandGlowsPurple.part
		part	:DroneHandGlowsBlue.part
		part	:Sparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	75
	End
End

## CORPSE FX ######################################################

Condition

	On	time
	Time	90

	Event
		EName	Rarm
		Type	Local
		At	T_chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part	:tendrilLightRays.part
		part	:LightFormCore.part
		part	:LightFormCoreBlue.part
		part	:LightFormCorePurple.part
		lifespan	70
	End
End

## GROUND FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName	reviveRing
		Type	start
		At	T_Root
		part	:FlatTendrils1.part
		part	:FlatTendrils1Purple.part
		part	:FlatTendrils1Blue.part
		part	:FlatTendrilsPurple.part
		part	:FlatTendrilsBlue.part
		Lifespan	180
	End

	Event
		Type	start
		At	T_Root
		BHVR	:Offset.bhvr		
		part	:BlackTendrilsRing.part
		part	:PurpleTendrilsRing.part
		part	:BlueTendrilsRing.part	
	End

	Event
		Type	Local 
		At	T_Root
		Part	:Gloom.part
		Part	:TendrilPurpleGloom.part
		Part	:TendrilBlueGloom.part
		Part	:TendrilGloom.part
		Pmagnet	T_Chest
	End
End

Condition
	On	Time
	Time	10
		
	Event
		Ename	Tendrils
		Type	Local	
		At	T_Root
		Part	:MainBaseFlatTendrils.part
		Part	:MainBaseFlatTendrilsPurple.part
		Part	:MainBaseFlatTendrilsBlue.part
		Lifespan	140
	End
End

########################################################

End				