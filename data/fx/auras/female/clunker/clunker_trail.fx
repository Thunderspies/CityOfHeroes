#############################################################
## Clunker_Trail.fx
#############################################################

FxInfo
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Chest
		ChildFx	:Child_OilDrips.fx
	End


	Event
		Type 	Local
		At	UarmL
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	UarmR
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	LarmL
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	LarmR
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	UlegL
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	UlegR
		ChildFx	:Child_OilDrips.fx
	End
End
#################

Condition
	On 		TRIGGERBITS
	TRIGGERBITS    	FEMALE
	DoMany		1

	Event
		Ename	OilPuddles
		Type 	Local
		At	Root
		Until	Air
		Flags	OneAtATime
		ChildFX	:GrandChild_OilPuddles.fx
	End

	Event
		Ename	OilPuddles2
		Type 	Local
		At	Root
		Until	Air
		Flags	OneAtATime
		ChildFX	:GrandChild_Puddle_Trail.fx
	End
End

#############################################################
End