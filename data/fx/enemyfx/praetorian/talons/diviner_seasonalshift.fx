#
FxInfo

	LifeSpan 150


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ToV_Diviner_SeasonalShift_01 300 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Part ENEMYFX\Praetorian\Talons\EnergyIn_Initial_Flat.part
			Part ENEMYFX\Praetorian\Talons\EnergyIn_Initial.part
			Part ENEMYFX\Praetorian\Talons\EnergyIn_Initial_powerball.part
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 43

		Event
			At Root
			Type Local
			Part ENEMYFX\Praetorian\Talons\Fire_FlareOut_Additive.part
			LifeSpan 0
		End

		Event
			At Root
			Type Local
			Part ENEMYFX\Praetorian\Talons\EnergyIn.part
			Part ENEMYFX\Praetorian\Talons\EnergyIn_Flat.part
			Part ENEMYFX\Praetorian\Talons\Fire_Additive.part
			Part ENEMYFX\Praetorian\Talons\Fire_Alpha.part
			Part ENEMYFX\Praetorian\Talons\Fire_CoreAdd.part
			Part ENEMYFX\Praetorian\Talons\Fire_Ground.part
			Part ENEMYFX\Praetorian\Talons\Glow_Heat.part
			Part ENEMYFX\Praetorian\Talons\Glow_Heat_Flat.part
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 103

		Event
			At Root
			Type Local
			Part ENEMYFX\Praetorian\Talons\Blast.part
			Part ENEMYFX\Praetorian\Talons\Blast_Big.part
			Part ENEMYFX\Praetorian\Talons\Blast_Big_Glow.part
			Part ENEMYFX\Praetorian\Talons\Fire_Splode.part
			Part ENEMYFX\Praetorian\Talons\Fire_Splode_Additive.part
		End

	End

End
