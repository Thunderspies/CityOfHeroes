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



################   1   ##########################


Condition
	On	Time
	Time	0
	Event
		EName 	binder1
		Type	local
		At	footR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinder1.bhvr
		Lifespan	45
	End

	Event
		EName 	Offset1
		Type	local
		At	binder1
		Bhvr	:binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset1
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset1
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   2   ##########################

Condition
	On	Time
	Time	0
	Event
		EName 	binder2
		Type	local
		At	footL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinder2.bhvr

		Lifespan	45
	End

	Event
		EName 	Offset2
		Type	local
		At	binder2
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset2
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset2
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   3   ##########################

Condition
	On	Time
	Time	10
	Event
		EName 	binder3
		Type	local
		At	llegR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinder1.bhvr

		Lifespan	40
	End

	Event
		EName 	Offset3
		Type	local
		At	binder3
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset3
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset3
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   4   ##########################

Condition
	On	Time
	Time	10
	Event
		EName 	binder4
		Type	local
		At	llegL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinder2.bhvr

		Lifespan	40
	End

	Event
		EName 	Offset4
		Type	local
		At	binder4
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset4
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset4
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   5   ##########################

Condition
	On	Time
	Time	15
	Event
		EName 	binder5
		Type	local
		At	Hips
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinder1.bhvr

		Lifespan	35
	End

	Event
		EName 	Offset5
		Type	local
		At	binder5
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset5
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset5
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   6   ##########################

Condition
	On	Time
	Time	20
	Event
		EName 	binder6
		Type	local
		At	Waist
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinder2.bhvr

		Lifespan	25
	End

	Event
		EName 	Offset6
		Type	local
		At	binder6
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset6
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset6
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End

################   7   ##########################

Condition
	On	Time
	Time	5
	Event
		EName 	binder7
		Type	local
		At	handR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr

		Lifespan	45
	End

	Event
		EName 	Offset7
		Type	local
		At	binder7
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset7
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset7
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   8   ##########################

Condition
	On	Time
	Time	5

	Event
		EName 	binder8
		Type	local
		At	handL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr

		Lifespan	45
	End

	Event
		EName 	Offset8
		Type	local
		At	binder8
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset8
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset8
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End

################   9   ##########################

Condition
	On	Time
	Time	15
	Event
		EName 	binder9
		Type	local
		At	larmR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr

		Lifespan	35
	End

	Event
		EName 	Offset9
		Type	local
		At	binder9
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset9
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset9
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   10   ##########################

Condition
	On	Time
	Time	15

	Event
		EName 	binder10
		Type	local
		At	larmL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr

		Lifespan	35
	End

	Event
		EName 	Offset10
		Type	local
		At	binder10
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset10
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset10
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End



##########################


Condition
	On	Time
	Time	0 #60

	Event
		EName	9
		Type	Local
		At	Head
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
#		Pmagnet	Head
	End

	Event
		EName	10
		Type	Local
		At	UArmL
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
#		Pmagnet	LArmL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
#		Pmagnet	LArmR
	End

	Event
		EName	11
		Type	Local
		At	LArmL
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
#		Pmagnet	WepL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
#		Pmagnet	WepR
	End

	Event
		EName	3
		Type	Local
		At	Hips
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
#		Pmagnet	Chest
	End



	Event
		EName	4
		Type	Local
		At	LLegL
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
		Pmagnet	ULegL
	End

	Event
		EName	7
		Type	Local
		At	LLegR
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
		Pmagnet	ULegR
	End

	Event
		EName	5
		Type	Local
		At	FootL
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
		Pmagnet	LLegL
	End

	Event
		EName	2
		Type	Local
		At	FootR
#		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke2.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
		Pmagnet	LLegR
	End
End

########################################################

End