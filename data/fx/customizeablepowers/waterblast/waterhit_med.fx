#############################################################
## EnergyBlastHit.fx
#############################################################

FxInfo

LifeSpan 40

#############################################################

Input
	InpName	Hips
End

###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	geom
		Type	local
		At	Chest
		bhvr	Behaviors\SpinHit2.bhvr
		geom	CenterDummy
		Sound 	WB_WaterHit_Med 100 100 .8
	End

	Event
		Ename	Splash
		At 	Chest
		Type 	Local
		Part 	:Hit_Ring_Heavy.part
		Part 	:Hit_Flash_Big.part
		Part 	:Hit_Flash_Heavy.part
		Part 	:Hit_Glow_Heavy.part
		Part 	:Hit_Streaks_Big.part
		Part 	:Splash_Splatter_Big.part
		Part 	:Splash_Splatter_Add_Big.part
		Part 	:Splash_Splatter2_Big.part
		Part 	:Splash_Splatter2_Add_Big.part
		Part 	:Splash_Splatter3_Big.part
		Part 	:Splash_Spray_Big.part
		LifeSpan 2
	End

End

Condition
	On	TRIGGERBITS
	TRIGGERBITS MALE

	Event
		Ename 	Drips
		type	Local
		At	Chest
		ChildFX :Hit_Drips_Child.fx
	End
End

Condition
	On	TRIGGERBITS
	TRIGGERBITS FEMALE

	Event
		Ename 	Drips
		type	Local
		At	Chest
		ChildFX :Hit_Drips_Child.fx
	End
End

Condition
	On	TRIGGERBITS
	TRIGGERBITS HUGE

	Event
		Ename 	Drips
		type	Local
		At	Chest
		ChildFX :Hit_Drips_Child.fx
	End
End


End
