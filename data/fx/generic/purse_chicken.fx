#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	WepR
End


########  Chicken body   ##########################################################

Condition
	On	Time
	Time	1




	Event
		EName 	Prime
		Type	Local 
		At	WepR 
		Geom	prop_chicken
		
	End



End



########  Chicken wings  ##########################################################

Condition
	On	Time
	Time	1




	Event
		EName 	Prime
		Type	Local 
		At	WepR 
		anim	fx_crazychickenwings
##		anim	fx_birdwings
		Bhvr	behaviors/chicken_purse.bhvr
		
	End



End




###### flying feathers  ###########################################


Condition
	On 	cycle
	Time 	20
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	WepR
		part1	generic/chickenfeathers.part
		Bhvr	behaviors/chicken_purse.bhvr
		
		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	20
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	WepR
		part1	generic/chickenfeathers.part
		Bhvr	behaviors/chicken_purse.bhvr
		

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	19
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	WepR
		part1	generic/chickenfeathers.part
		Bhvr	behaviors/chicken_purse.bhvr
		

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	21
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	WepR
		part1	generic/chickenfeathers.part
		Bhvr	behaviors/chicken_purse.bhvr
		

		lifespan	25
	End
	
End


Condition
	On 	cycle
	Time 	22
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	WepR
		part1	generic/chickenfeathers.part
		Bhvr	behaviors/chicken_purse.bhvr
		

		lifespan	25
	End
	

End

Condition
	On 	cycle
	Time 	17
	Chance	.12

	Event
		EName	3
		Type	Local
		At	WepR
		part1	generic/chickenfeathers.part
		Bhvr	behaviors/chicken_purse.bhvr
		
		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	17
	Chance	.12

	Event
		EName	3
		Type	Local
		At	WepR
		part1	generic/chickenfeathers.part
		Bhvr	behaviors/chicken_purse.bhvr
		

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.12

	Event
		EName	3
		Type	Local
		At	WepR
		part1	generic/chickenfeathers.part
		Bhvr	behaviors/chicken_purse.bhvr
		
		lifespan	25
	End


End

#####################################################################################





End
##################################

