#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan	200

########################################################################


Condition
	On 	Time
	Time 	0

	Event
		EName	hookup
		Type	local
		At	T_Chest
	End

	Event
		EName	Prime
		Type	startPositOnly
		At	hookup
		Geom	GEO_WepR_Shuriken
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		Part1	ENEMYFX\Tsoo\ShurikenStreaks01.part
		Magnet	Chest
		LookAt	Chest
		Pother	Chest
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		Ename 	Boom
		Type 	local
		At 	Chest
		ChildFX :TrickShot_Pet_Child.fx
	End
End

####################################################################

End

