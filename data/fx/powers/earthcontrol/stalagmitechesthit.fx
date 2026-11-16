#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	Chest
End

LifeSpan	200

#################################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	spikes
		Type	local
		At	chest
		geom	EarthDummy	
		Bhvr	behaviors/StalagtiteCore.bhvr
		
	End

End


Condition
	On 	Time
	Time 	23

	Event
		EName 	Stonesickle01
		Type	local
		At	spikes
		 AltPiv 1
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite01
	End

End

Condition
	On 	Time
	Time 	27

	Event
		EName 	Stonesickle02
		Type	local
		At	spikes
		 AltPiv 2
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite02
		
	End


End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Stonesickle03
		Type	local
		At	spikes
		 AltPiv 3
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite03
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName 	Stonesickle04
		Type	local
		At	spikes
		 AltPiv 4
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite04	
	End


	Event
		EName 	Stonesickle10
		Type	local
		At	spikes
		 AltPiv 4
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite10
	End
End

Condition
	On 	Time
	Time 	22

	Event
		EName 	Stonesickle05
		Type	local
		At	spikes
		 AltPiv 5
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite05	
	End

End

Condition
	On 	Time
	Time 	26

	Event
		EName 	Stonesickle06
		Type	local
		At	spikes
		 AltPiv 6
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite06
	End

End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Stonesickle07
		Type	local
		At	spikes
		 AltPiv 6
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite07
	End

End


Condition
	On 	Time
	Time 	22

	Event
		EName 	Stonesickle08
		Type	local
		At	spikes
		 AltPiv 2
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite08
	End

End


Condition
	On 	Time
	Time 	24

	Event
		EName 	Stonesickle09
		Type	local
		At	spikes
		 AltPiv 4
		Bhvr	behaviors/Stalagtite.bhvr
		geom	FX_Stalagtite09
	End

End



#############################################################
##Kills
##########################################################


Condition
		On 	Time
		Time 	100


	Event
		Ename	Stonesickle01
		Type	Destroy

	End
End

Condition
		On 	Time
		Time 	102
	Event
		Ename	Stonesickle02
		Type	Destroy

	End

End

Condition
		On 	Time
		Time 	107
	Event
		Ename	Stonesickle03
		Type	Destroy

	End

End


Condition
		On 	Time
		Time 	105
	Event
		Ename	Stonesickle04
		Type	Destroy

	End

End

Condition
		On 	Time
		Time 	103
	Event
		Ename	Stonesickle05
		Type	Destroy

	End

End


Condition
		On 	Time
		Time 	107
	Event
		Ename	Stonesickle06
		Type	Destroy

	End

End	

Condition
		On 	Time
		Time 	104
	Event
		Ename	Stonesickle07
		Type	Destroy

	End

End

Condition
		On 	Time
		Time 	113
	Event
		Ename	Stonesickle08
		Type	Destroy

	End

End

Condition
		On 	Time
		Time 	115
	Event
		Ename	Stonesickle09
		Type	Destroy

	End

End

Condition
		On 	Time
		Time 	117
	Event
		Ename	Stonesickle10
		Type	Destroy

	End

End