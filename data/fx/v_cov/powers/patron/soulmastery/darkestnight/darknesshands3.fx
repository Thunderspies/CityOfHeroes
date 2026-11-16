#########################################################
## Darkness Hands - Used for "Darkest Night" powers
#########################################################

FxInfo

Lifespan 220

## SOUNDS #######################################################

Condition

	On	Time
	Time	0

	Event
		Type 	Local
		At	chest
		Sound miasma_loop 50.0 50.0 .4
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End

Condition

	On	Time
	Time	0

	Event
		Type 	Local
		At	chest
		Sound miasma_loop 50.0 50.0 .4
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End

#######################################################
#		EFFECTS
#######################################################


Condition
	On	Time
	Time	3
	Event
		EName 	Rbinder7
		Type	local
		At	UarmR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
		Lifespan	20
	End

	Event
		EName 	ROffset7
		Type	local
		At	Rbinder7
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	ROffset7
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	ROffset7
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End

Condition
	On	Time
	Time	11
	Event
		EName 	Rbinder9
		Type	local
		At	LarmR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr

		Lifespan	20
	End

	Event
		EName 	ROffset9
		Type	local
		At	Rbinder9
		Bhvr	:binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	ROffset9
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	ROffset9
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


Condition
	On	Time
	Time	13

	Event
		EName 	HandR
		Type	local
		At	WepR
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow1.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire2.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke1.part

		Lifespan	9
	End


End

###########################
#	Left Arm	  #
###########################

Condition
	On	Time
	Time	3
	Event
		EName 	Lbinder7
		Type	local
		At	UarmL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			BhvrOverride
				InitialVelocity		-0.02 0.0 0.0
				InitialVelocityJitter	0.005 0.00 0.00
			End
		Lifespan	20
	End

	Event
		EName 	LOffset7
		Type	local
		At	Lbinder7
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			BhvrOverride
				PositionOffset		-1 0 2
				Spin			0.0 0.0 0.1
				SpinJitter		0.0 0.0 0.05
			End
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	LArm1
		Type	local
		At	LOffset7
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	LArm2
		Type	local
		At	LOffset7
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End

Condition
	On	Time
	Time	11
	Event
		EName 	Lbinder9
		Type	local
		At	LarmL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			BhvrOverride
				InitialVelocity		-0.02 0.0 0.0
				InitialVelocityJitter	0.005 0.00 0.00
			End
		Lifespan	20
	End

	Event
		EName 	LOffset9
		Type	local
		At	Lbinder9
		Bhvr	:binderOffset.bhvr
			BhvrOverride
				PositionOffset		-1 0 2
				Spin			0.0 0.0 0.1
				SpinJitter		0.0 0.0 0.05
			End
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	LArm1
		Type	local
		At	LOffset9
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	LArm2
		Type	local
		At	LOffset9
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


Condition
	On	Time
	Time	13

	Event
		EName 	HandL
		Type	local
		At	WepL
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow1.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire2.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke1.part

		Lifespan	9
	End


End




##########################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		Sound miasmaburst 100 100 .8
		Lifespan	90
	End

	Event
		Type	Local
		At	WepL
		Sound miasma_loop 100 100 .5
		Lifespan	90
	End

	Event

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Part1	Powers\DarknessControl\DarkHand.part
		Part3	Powers\DarknessControl\DarkHandsEmber.part
		LifeSpan	90
	End

	Event
		ENAME	Base
		Type	local
		At	WepL
		Part1	Powers\DarknessControl\DarkHand.part
		Part3	Powers\DarknessControl\DarkHandsEmber.part
		LifeSpan	90
	End
End

#########################################################

End