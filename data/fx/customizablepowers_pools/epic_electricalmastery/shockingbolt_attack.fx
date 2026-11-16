#                                                          
FxInfo

	LifeSpan 75


###########################################################

	Condition
		On Time
		Time 9

		Event
			At WepL
			Type Local
			Sound lightningfield 80 80 0.86
		End

	End

	Condition
		On Time
		Time 20

		Event
			At WepL
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :WitchHaze.bhvr
			Part :WitchElectricityBlastHaze.part
		End

		Event
			At WepL
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt05.part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 25

		Event
			At WepR
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 28

		Event
			At WepR
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :ChargedBolt03.part
			Part :ChargedBolt04.part
		End

	End

	Condition
		On Time
		Time 32

		Event
			At WepL
			Type Start
			EName Bolt
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 35

		Event
			At WepR
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :ChargedBolt03.part
			Part :ChargedBolt04.part
		End

	End

	Condition
		On Time
		Time 39

		Event
			At WepL
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 42

		Event
			At WepL
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :ChargedBolt03.part
			Part :ChargedBolt04.part
		End

	End

	Condition
		On Time
		Time 46

		Event
			At WepL
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 20

		Event
			At WepR
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt05.part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 25

		Event
			At WepL
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 28

		Event
			At WepR
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :ChargedBolt03.part
			Part :ChargedBolt04.part
		End

	End

	Condition
		On Time
		Time 33

		Event
			At WepR
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 36

		Event
			At WepL
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :ChargedBolt03.part
			Part :ChargedBolt04.part
		End

	End

	Condition
		On Time
		Time 40

		Event
			At WepL
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 43

		Event
			At WepR
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :ChargedBolt03.part
			Part :ChargedBolt04.part
		End

	End

	Condition
		On Time
		Time 46

		Event
			At WepR
			Type Start
			POther Target
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchBoltMega.Part
			Part :WitchBolt2Mega.Part
			Part :ChargedBolt01.part
			Part :ChargedBolt02.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 10

		Event
			At WepL
			Type Local
			EName Thingy
			Part :ElectricityControl01Witches.part
			Part :ElectricityControl02Witches.part
			Part :ElectricityControl01.part
			Part :ElectricityControl02.part
			Part :ElectricityInnerGlow.part
			Sound lightningbolt3 80 30 0.76
		End

		Event
			At WepL
			Type Local
			EName Thingy1
			Part :ElectricityControl01Witches.part
			Part :ElectricityControl02Witches.part
			Part :ElectricityControl01.part
			Part :ElectricityControl02.part
			Part :ElectricityInnerGlow.part
		End

		Event
			At WepR
			Type Local
			EName ThingyA
			#Bhvr :WitchLightningOffset.bhvr
			Part :ElectricityOuterGlow.part
			Part :ElectricityArch.part
			Part :ElectricityRing.part
		End

		Event
			At WepL
			Type Local
			EName ThingyB
			#Bhvr :WitchLightningOffset.bhvr
			Part :ElectricityOuterGlow.part
			Part :ElectricityArch.part
			Part :ElectricityRing.part
		End

	End

	Condition
		On Time
		Time 15

		Event
			At WepL
			Type Local
			EName BurstR
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchSparks.part
			LifeSpan 20
		End

		Event
			At WepL
			Type Local
			EName BurstL
			#Bhvr :WitchLightningOffset.bhvr
			Part :WitchSparks.part
			LifeSpan 20
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

	Condition
		On Time
		Time 55

		Event
			Type Destroy
			EName Thingy
		End

		Event
			Type Destroy
			EName Thingy1
		End

		Event
			Type Destroy
			EName ThingyA
		End

		Event
			Type Destroy
			EName ThingyB
		End

	End

End
