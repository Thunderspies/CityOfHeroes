#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	MeteorAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset		0 600 -500
			InitialVelocity		0 -12 10
			Scale			0.5 0.5 0.5
			#InitialVelocityJitter	0.5 1.5 2
		End
	End
End


Condition
	On		Time
	Time		0

	Event
		Ename	Fireball
		Type	Local
		At	MeteorAnchor
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Meteor_Attack_Rock.part
		Part	:Meteor_Attack_Core.part
		Part	:Meteor_Attack_Secondary.part
		Part	:Meteor_Attack_SmokeTrail.part
		Lifespan 50
	End
End

Condition
	On		Time
	Time		40

	Event
		Ename	Fireball
		Type	Local
		At	MeteorAnchor
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Meteor_Attack_SmokeTrail2.part
		Lifespan 10
	End
End

#############################################################

End

