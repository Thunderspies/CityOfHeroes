#########################################################
##	tenticle fun yea!
########################################################
FxInfo

Input  
	Inpname	Root
End


##################################################################

Condition
	On	Time
	Time	0




	Event
		EName 	Prime
		Type	Local 
		At	Root 
		
		Bhvr	behaviors/ScaleTenticle.bhvr
		Anim	FX_tenticlebase
	End


	Event
		EName 	tenticle_1
		Type	Local 
		At	Prime 

		Bhvr	behaviors/ScaleTenticle.bhvr

		AnimPiv Chest
		Anim	FX_darktenticle_B
	End



	Event
		EName 	tenticle_2
		Type	Local 
		At	Prime 

		Bhvr	behaviors/ScaleTenticle.bhvr

		AnimPiv Neck
		Anim	FX_darktenticle
	End

	Event
		EName 	tenticle_3
		Type	Local 
		At	Prime 

		Bhvr	behaviors/ScaleTenticle.bhvr

		AnimPiv Head
		Anim	FX_darktenticle_A
	End

	Event
		EName 	tenticle_4
		Type	Local 
		At	Prime 

		Bhvr	behaviors/ScaleTenticle.bhvr

		AnimPiv Hair
		Anim	FX_darktenticle
	End

	Event
		EName 	tenticle_5
		Type	Local 
		At	Prime 

		Bhvr	behaviors/ScaleTenticle.bhvr
		
		AnimPiv Waist
		Anim	FX_darktenticle_A
	End




End

End
##################################

