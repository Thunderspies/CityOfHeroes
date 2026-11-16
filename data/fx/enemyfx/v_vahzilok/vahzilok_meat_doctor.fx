#########################################################
##	template

FxInfo

Flags  InheritAlpha   InheritAnimScale

#########################################################
####################### back pack ###########################
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Back	
		Bhvr	:Vahzilok_Backpack_Offset.bhvr
		Anim	V_Vahzilok_Meat_Doctor_Backpack
		
	End

	Event
		EName	CrossBowGEO
		Type	Local
		At	LarmL
		
		bhvr	:Vahzilok_CrossBow_Offset.bhvr
		Geom	GEO_MeatDoc_CrossBow
	End

End

Condition
	On	Time
	Time	0

	Event
		EName	SawBlade
		Type	Local
		At	Chest
		
		ChildFX	V_COV\EnemyPowers\Vahzilok\MeatDoctor_BuzzSaw.fx
	End
End



End

