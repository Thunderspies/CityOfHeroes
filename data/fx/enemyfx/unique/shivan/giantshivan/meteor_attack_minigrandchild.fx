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
			PositionOffset		0 150 -125
			#StartJitter		5 0 5
			InitialVelocity		0 -3 2.5
			InitialVelocityJitter	0 0.2 0.2
		End
	End
End


Condition
	On		Time
	Time		0
	Repeat	5

	Event
		Ename	Fireball
		Type	Local
		At	MeteorAnchor
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Meteor_Attack_Pebble.part
		Lifespan 50
	End
End



Condition
	On		Time
	Time		40

	Event
		Ename	Smoke
		Type	Local
		At	MeteorAnchor
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Meteor_Attack_MiniTrail.part
		Lifespan 10
	End
End


#############################################################

End

