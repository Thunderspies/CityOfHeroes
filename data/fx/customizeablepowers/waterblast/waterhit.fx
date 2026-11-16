#############################################################
## EnergyBlastHit.fx
#############################################################

FxInfo

LifeSpan 40

#############################################################

Input
	InpName	Chest
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
	End

	Event
		EName 	hit2
		Type 	local
		At 	Chest
		Part 	:Hit_Streaks.part
		Part 	:Splash_Flash.part
		Part 	:Splash_Splatter.part
		Part 	:Splash_Splatter_Add.part
		Part 	:Splash_Splatter2.part
		Part 	:Splash_Splatter2_Add.part
		Part 	:Splash_Splatter3.part
		Part 	:Splash_Spray.part
		LifeSpan 2
	End

	Event
		Ename	SFX
		Type	Local
		At	Chest
		Sound 	WB_WaterHit 100 100 .8
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
