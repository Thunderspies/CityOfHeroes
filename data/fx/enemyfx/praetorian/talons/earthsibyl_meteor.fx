#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo
LifeSpan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	MeteorAnchor
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 600 -500
			InitialVelocity		0 -12 10
			Scale			0.5 0.5 0.5
			TrackRate		6.0
			#InitialVelocityJitter	0.5 1.5 2
		End
	End
End


Condition
	On		Time
	Time		0

	Event
		Type	Start
		At	MeteorAnchor
		EName 	Prime
		LookAt 	Target
		Magnet 	Target
		Bhvr 	behaviors\Braz\Projectile_FastWithUp_02.bhvr
		Part	:Meteor_Attack_Rock.part
		Part	:Meteor_Attack_Core.part
		Part	:Meteor_Attack_Secondary.part
		Part	:Meteor_Attack_SmokeTrail.part
		Part	:Meteor_Attack_SmokeTrail2.part
		Lifespan 300
	End
End

Condition
	On PrimeHit

	Event
		Type Destroy
		EName Prime
	End

	Event
		Type	Local
		At	T_Root
		ChildFX	:MeteorImpact_CHILD.fx
	End

End

#############################################################

End

