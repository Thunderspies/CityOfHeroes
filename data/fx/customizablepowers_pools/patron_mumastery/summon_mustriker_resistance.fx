###########################################################

FxInfo

###########################################################

	Condition
		On Time
		Time 0
		
		Event
			At Root
			Type Local
			CHILDFX V_COV\EnemyPowers\Arachnos\mu_LightningArmor.fx
		End
		
		Event
			At Root
			Type Local
			EName Hookup
			Bhvr Powers/ItemsAndTempPowers/OverheadOffset.bhvr
			Sound SSthunder2 180 180 0.9
		End

		Event
			At Hookup
			Type Local
			PMagnet root
			Part CustomizeablePowers\ElectricalMelee\LightningStrike2.part
		End

		Event
			At Root
			Type Local
			EName Hookup2
			Bhvr Powers/ItemsAndTempPowers/OverheadOffset2.bhvr
		End

		Event
			At Root
			Type Start
			EName node01
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 50 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node01
			Type Start
			EName ChargedBolt1
			POther node02
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		#On Time
		#Time 1

		Event
			At Root
			Type Start
			EName node02
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 40 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node02
			Type Start
			EName ChargedBolt2
			POther node03
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		#On Time
		#Time 1

		Event
			At Root
			Type Start
			EName node03
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 30 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node03
			Type Start
			EName ChargedBolt3
			POther node04
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		#On Time
		#Time 1

		Event
			At Root
			Type Start
			EName node04
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 20 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node04
			Type Start
			EName ChargedBolt4
			POther node05
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		#On Time
		#Time 1

		Event
			At Root
			Type Start
			EName node05
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 10 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node05
			Type Start
			EName ChargedBolt6
			POther root
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Hookup
			Type Local
			PMagnet root
			Part CustomizeablePowers\ElectricalMelee\LightningStrike3.part
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Hookup2
			Type Local
			PMagnet root
			Part CustomizeablePowers\ElectricalMelee\LightningStrike2.part
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Hookup
			Type Local
			PMagnet root
			Part CustomizeablePowers\ElectricalMelee\LightningStrike.part
		End

		Event
			At Root
			Type Start
			EName node01
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 50 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node01
			Type Start
			EName ChargedBolt1
			POther node02
			Part :Red_ChargedBolt01a.Part
			Part CustomizeablePowers\ElectricalBlast\ChargedBolt03.Part
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		#On Time
		#Time 1

		Event
			At Root
			Type Start
			EName node02
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 40 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node02
			Type Start
			EName ChargedBolt2
			POther node03
			Part :Red_ChargedBolt01a.Part
			Part CustomizeablePowers\ElectricalBlast\ChargedBolt03.Part
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		#On Time
		#Time 1

		Event
			At Root
			Type Start
			EName node03
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 30 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node03
			Type Start
			EName ChargedBolt3
			POther node04
			Part :Red_ChargedBolt01a.Part
			Part CustomizeablePowers\ElectricalBlast\ChargedBolt03.Part
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		#On Time
		#Time 1

		Event
			At Root
			Type Start
			EName node04
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 20 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node04
			Type Start
			EName ChargedBolt4
			POther node05
			Part :Red_ChargedBolt01a.Part
			Part CustomizeablePowers\ElectricalBlast\ChargedBolt03.Part
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
		End

	End

	Condition
		#On Time
		#Time 1

		Event
			At Root
			Type Start
			EName node05
			BhvrOverride
				StartJitter 3 0 3
				PositionOffset 0 10 0
			End
			Geom centerdummy
		End

		Event
			AltPiv 1
			At node05
			Type Start
			EName ChargedBolt6
			POther root
			Part :Red_ChargedBolt01a.Part
			Part CustomizeablePowers\ElectricalBlast\ChargedBolt03.Part
			Part :Red_ChargedBolt01a.Part
			Part :Red_ChargedBolt02a.Part
			Part :Red_ChargedBolt05a.Part
			Part :Red_ChargedBolt06a.Part
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Chest
			Type Local
			Part CustomizeablePowers\ElectricalMelee\LightningStrikeGlow.part
			Part CustomizeablePowers\ElectricalMelee\LightningStrikeHaze.part
			Part CustomizeablePowers\ElectricalMelee\LightningStrikeHazeWhite.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Chest
			Type Local
			Part :LightningStreeks.part
			Part :LightningStreeksDark.part
			LifeSpan 12
		End

	End

Condition
	On	DEATH

	Event
		Type	Start
		At	Root
		ChildFX	:Summon_Attack.fx
		Lifespan 190
	End
End