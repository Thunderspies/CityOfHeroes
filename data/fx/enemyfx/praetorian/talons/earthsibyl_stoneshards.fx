#
FxInfo

	Flags InheritAnimScale

	LifeSpan 80


###########################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	ToV_EarthSibyl_StoneShardsPre_01 200 100 .8
	End
End

Condition
	On 	Time
	Time	60
	Event
		Type	Local
		At	Root
		Sound 	ToV_EarthSibyl_StoneShards_01 200 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Part ENEMYFX\Praetorian\Talons\Dust_GroundChargeUp.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Chest
			Type Local
			Bhvr :RockArms.bhvr
			Geom StoneArmor_CHEST02
			LifeSpan 55
		End

		Event
			At UArmL
			Type Local
			Bhvr :RockArms.bhvr
			Geom StoneArmor_UARML02
			Part ENEMYFX\Praetorian\Talons\Dust_RockArms.part
			LifeSpan 55
		End

		Event
			At UArmR
			Type Local
			Bhvr :RockArms.bhvr
			Geom StoneArmor_UARMR02
			Part ENEMYFX\Praetorian\Talons\Dust_RockArms.part
			LifeSpan 55
		End

	End

	Condition
		On Time
		Time 15

		Event
			At LArmR
			Type Local
			Bhvr :RockArms.bhvr
			Geom StoneArmor_LARMR02
			Part ENEMYFX\Praetorian\Talons\Dust_RockArms.part
			LifeSpan 45
		End

		Event
			At LArmL
			Type Local
			Bhvr :RockArms.bhvr
			Geom StoneArmor_LARML02
			Part ENEMYFX\Praetorian\Talons\Dust_RockArms.part
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 60
		Repeat 16

		Event
			At Hips
			Type Local
			ChildFx :CHILD_StoneShards_Attack.fx
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 60
		Repeat 6

		Event
			At Chest
			Type Local
			ChildFx V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx
			LifeSpan 100
		End

		Event
			At Chest
			Type Local
			ChildFx V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx
			LifeSpan 100
		End

	End

	Condition
		On Time
		Time 60

		Event
			At Root
			Type Local
			EName RingAnchor
			Bhvr ENEMYFX\Praetorian\Talons\Ground_Dust_Expand.bhvr
			BhvrOverride
				PositionOffset 0 3.5 0
			End
			Part ENEMYFX\Praetorian\Talons\Stone_Spikes_Expanding.part
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
