#                                                          
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 0

		Event
			At HandR
			Type Local
			EName Thingy
			Part :Red_ElectricityControl01.part
			Part :Red_ElectricityControl02.part
			Part :Red_ElectricityInnerGlow.part
			Part :Red_ElectricityControl01white.part
			Part :Red_ElectricityControl02white.part
			Sound elecwindup_loop 80 80 0.5
		End

		Event
			At HandR
			Type Local
			EName ThingyA
			Part :Red_ElectricityControl01.part
			Part :Red_ElectricityControl02.part
			Part :Red_ElectricityInnerGlow.part
			Part :Red_ElectricityControl01white.part
			Part :Red_ElectricityControl02white.part
		End

	End

	Condition
		On Time
		Time 5

		Event
			At HandR
			Type Start
			EName Prime
			Magnet Target
			Bhvr Behaviors/Projectile4.bhvr
		End

		Event
			At HandR
			Type Start
			POther Target
			Part :Red_ChargedBolt06b.part
			Part :Red_ChargedBolt05b.part
			Part :Red_ChargedBolt01b.part
			Part :Red_ChargedBolt02b.part
			Part :Red_ChargedBolt06b.part
			Part :Red_ChargedBolt05b.part
			Part :Red_ChargedBolt01b.part
			Part :Red_ChargedBolt02b.part
			Part :Red_ChargedBolt06.part
			Part :Red_ChargedBolt05.part
			Part :Red_ChargedBolt01.part
			Part :Red_ChargedBolt02.part
			Part :Red_ChargedBolt06.part
			Part :Red_ChargedBolt05.part
			Part :Red_ChargedBolt01.part
			Part :Red_ChargedBolt02.part
			Sound elecboltquick 88 88 0.8
		End

	End

	Condition
		On Time
		Time 8

		Event
			At HandR
			Type Start
			POther Target
			Part :Red_ChargedBolt06b.part
			Part :Red_ChargedBolt05b.part
			Part :Red_ChargedBolt01b.part
			Part :Red_ChargedBolt02b.part
			Part :Red_ChargedBolt06b.part
			Part :Red_ChargedBolt05b.part
			Part :Red_ChargedBolt01b.part
			Part :Red_ChargedBolt02b.part
			Part :Red_ChargedBolt06.part
			Part :Red_ChargedBolt05.part
			Part :Red_ChargedBolt01.part
			Part :Red_ChargedBolt02.part
			Part :Red_ChargedBolt06.part
			Part :Red_ChargedBolt05.part
			Part :Red_ChargedBolt01.part
			Part :Red_ChargedBolt02.part
		End

	End

	Condition
		On Time
		Time 12

		Event
			At HandR
			Type Start
			POther Target
			Part :Red_ChargedBolt06b.part
			Part :Red_ChargedBolt05b.part
			Part :Red_ChargedBolt01b.part
			Part :Red_ChargedBolt02b.part
			Part :Red_ChargedBolt06b.part
			Part :Red_ChargedBolt05b.part
			Part :Red_ChargedBolt01b.part
			Part :Red_ChargedBolt02b.part
			Part :Red_ChargedBolt06.part
			Part :Red_ChargedBolt05.part
			Part :Red_ChargedBolt01.part
			Part :Red_ChargedBolt02.part
			Part :Red_ChargedBolt06.part
			Part :Red_ChargedBolt05.part
			Part :Red_ChargedBolt01.part
			Part :Red_ChargedBolt02.part
		End

	End

End
