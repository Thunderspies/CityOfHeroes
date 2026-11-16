#############################################################
## XXLargeFireBall.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	ShivanGiant_MeteorCrash_01 200 150 .8
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Origin
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		Bhvr	behaviors/CameraShake02.bhvr
		At	Origin
		Part	IncarnatePowers\judgment\Judgment_Pyronic_Flash.part
		Part	IncarnatePowers\judgment\Judgment_Pyronic_Flash2.part
		Part	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_Fire_Mult.part
		Part1	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_Fire.part
		Part1	IncarnatePowers\judgment\Judgment_Pyronic_AreaAmber.part
		Lifespan 3
	End

	Event
		Ename	ScortchMark
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0 0.5 0
		End

	End
End

Condition
	On	Time
	Time	0


	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	Origin
		BhvrOverride
          		PositionOffset 0 0.5 0

         	End
		Part	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_Chunks.part
		Part	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_FireB_Mult.part
		Part	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_FireB.part
		Part	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_Fire_Floor.part
		Part	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_DustPillar.part
	#	Part	:Meteor_Attack_Residue.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	6
	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	Origin
		Part	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_FireB_Mult.part
		Part	ENEMYFX\Unique\Shivan\GiantShivan\Meteor_Impact_FireB.part
		Lifespan 3
	End

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	Origin
		Part1	IncarnatePowers\judgment\Judgment_Pyronic_Smoke_Range.part
		Part1	IncarnatePowers\judgment\Judgment_Pyronic_SmokeB.part
		Part1	IncarnatePowers\judgment\Judgment_Pyronic_Smoke_Lingering.part

		Lifespan 10
	End

End

Condition
	On	Time
	Time	0
	Repeat	3

	Event
		Ename	MeteoBreak
		Type	Local
		At	Origin
		Bhvr	Behaviors/Braz/Rockfade.bhvr
		BhvrOverride
			StartColor		150 150 150
			PositionOffset		0.0 0.5 0.0
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 2 0.0
			InitialVelocityJitter	0.8 1 0.8
			PyrRotateJitter		180 180 180
			Gravity			0.2
		End
		Geom	Tintable_FX_StoneWall_07
		Geom	Tintable_FX_StoneWall_11
	End

	Event
		Ename	MeteoBreak2
		Type	Local
		At	Origin
		Bhvr	Behaviors/Braz/Rockfade.bhvr
		BhvrOverride
			StartColor		150 150 150
			PositionOffset		0.2 0.5 0.2
			Scale			0.5 0.4 0.5
			InitialVelocity		0.0 1 0.0
			InitialVelocityJitter	1 0.5 1
			PyrRotateJitter		180 180 180
			Gravity			0.1
		End
		Geom	Tintable_FX_StoneWall_07
		Geom	Tintable_FX_StoneWall_11
	End

End
#############################################################

End