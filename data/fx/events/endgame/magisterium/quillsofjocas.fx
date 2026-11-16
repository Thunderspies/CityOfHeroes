#
FxInfo

	Flags InheritAnimScale

	LifeSpan 120


###########################################################

#Condition
#	On 	Time
#	Time	5
#	Event
#		Type	Local
#		At	Root
#		Sound 	ToV_EarthSibyl_StoneShardsPre_01 200 100 .8
#	End
#End
#
#Condition
#	On 	Time
#	Time	60
#	Event
#		Type	Local
#		At	Root
#		Sound 	ToV_EarthSibyl_StoneShards_01 200 100 .8
#	End
#End

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	Magisterium_QuillsOfJocas_01 100 100 .8
	End
End

	Condition
		On Time
		Time 8

		Event
			At Root
			Type Local
			ChildFX :Quills_Attack_Hands_CHILD.fx
			LifeSpan 70
		End

	End

	Condition
		On Time
		Time 55

		Event
			Ename Anchor
			At Chest
			Type Start
			Part :Quills_Flash.part
			Part :Quills_Boils.part
			Part :Hit_Shockwave_Drone.part
			Part :Quills_Smoke.part
			LifeSpan 15
		End
	End

	Condition
		On Cycle
		Time 3
		Repeat 6

		Event
			At Anchor
			Type Local
			ChildFx :Quills_Attack_CHILD.fx
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 55

		Event
			At Root
			Type Local
			EName RingAnchor
			Bhvr ENEMYFX\Praetorian\Talons\Ground_Dust_Expand.bhvr
			BhvrOverride
				PositionOffset 0 3.5 0
			End
		#	Part ENEMYFX\Praetorian\Talons\Stone_Spikes_Expanding.part
			Part ENEMYFX\Praetorian\Talons\Dust_Ring_Expanding.part
			LifeSpan 0
		End

		Event
			At Chest
			Type Start
			EName PHYSICS-ExplosiveForce
			BhvrOverride
				physForceType Out
				physForceRadius 15
				physForcePower 600
				physForcePowerJitter 75
			End
			LifeSpan 1
		End

	End

End
