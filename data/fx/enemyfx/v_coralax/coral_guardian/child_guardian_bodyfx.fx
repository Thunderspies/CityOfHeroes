#########################################################
##	iceweapon
########################################################
FxInfo

Input
	Inpname	Origin
End

Lifespan 90

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Main
		Type	local
		At	Root
		Anim	V_Coral_Guardian_Boss_Tintable
		BhvrOverride
			Alpha 1
		End
		Lifespan	85
	End
End

Condition
	On	Time
	Time	10

	Event
		EName 	Anchor_LLegR
		Type	PositOnly
		At	Main
		AnimPiv	UlegR
		Lifespan	100
	End

	Event
		EName 	Anchor_LLegL
		Type	PositOnly
		At	Main
		AnimPiv	LLegL
		Lifespan	100
	End

	Event
		EName 	Anchor_HandR
		Type	PositOnly
		At	Main
		AnimPiv	HandR
		Lifespan	100
	End

	Event
		EName 	Anchor_HandL
		Type	PositOnly
		At	Main
		AnimPiv	HandL
		Lifespan	100
	End

	Event
		EName	Anchor_LArmR
		Type	PositOnly
		At	Main
		AnimPiv	LArmR
		Lifespan	100
	End

	Event
		Type	PositOnly
		At	Main
		AnimPiv	LarmR
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_HandR
		Lifespan	40
	End

	Event

		Type	PositOnly
		At	Main
		AnimPiv UArmR
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_LArmR
		Lifespan	40
	End

	Event
		Type	PositOnly
		At	Main
		AnimPiv	UlegR
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_LLegR
		Lifespan	40
	End

	Event
		EName	Anchor_LArmL
		Type	PositOnly
		At	Main
		AnimPiv	LarmL

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_HandL
		Lifespan	40
	End

	Event
		Type	PositOnly
		At	Main
		AnimPiv	UarmL

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_LArmL
		Lifespan	40
	End


	Event
		Type	PositOnly
		At	Main
		AnimPiv	ULegL
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		POther	Anchor_LLegL
		Lifespan	40
	End




End

Condition
	On 	Time
	Time 	15

	Event
		EName	Target_UArmR
		Type	PositOnly
		At	Main
		AnimPiv	UArmR
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		Lifespan	60
	End

	Event
		EName	Anchor_LArmR
		Type	PositOnly
		At	Main
		AnimPiv	LArmR
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		Lifespan	60
	End

	Event
		Type	local
		At	PositOnly
		AnimPiv	UlegR
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_LLegR
		Lifespan	60
	End


	Event
		Type	PositOnly
		At	Main
		AnimPiv	UArmL
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_LArmL
		Lifespan	60
	End

	Event
		Type	PositOnly
		At	Main
		AnimPiv	LarmL
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_HandL
		Lifespan	60
	End

	Event
		Type	PositOnly
		At	Main
		AnimPiv	UlegL
		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	Anchor_LLegL
		Lifespan	60
	End
End

########################################################

End
