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
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		0 250 800
			StartJitter 		100 100 600
			InitialVelocity		0 -3 -2.5
			InitialVelocityJitter	0.5 1.5 2
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
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:Meteor_Fall.part
		Part2	:Meteor_Fire_Core.part
		Part3	:Meteor_Fire_Secondary.part
		Part4	:Meteor_SmokeTrail.part
		Lifespan 140
	End
End

#############################################################

End

