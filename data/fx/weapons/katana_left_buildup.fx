#########################################################
##	jack
##
FxInfo

Flags InheritAlpha DontSuppress


Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local
		At	WepL
		Geom	GEO_WepL_Katana_2
		Sound swordshing 70 70 .6
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime

		BhvrOverride
			PositionOffset	 0 0 .3
		End

		Bhvr	Behaviors/Fade.bhvr

		Part1	V_COV/Powers/Ninjas/fireweapon3follow.part
		Part1	V_COV/Powers/Ninjas/fireweapon1.part
		Part2	V_COV/Powers/Ninjas/fireweapon2.part
		Part3	V_COV/Powers/Ninjas/fireweapon3.part
		Part4	V_COV/Powers/Ninjas/fireweaponWhitness.part
		Part3	Powers/FireControl/fireweaponspark.part

		LifeSpan		30


	End


End