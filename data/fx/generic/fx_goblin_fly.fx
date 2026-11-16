#########################################################
##	template

FxInfo


Flags 


######  uprooted goblin  #####################

Condition
	On 	Time
	Time 	30


	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Anim	FX_goblin_fly
		bhvr	behaviors/Goblin_UpRoot.bhvr
		Lifespan	46

	End

End 


###### Projectile Goblin  #################################

Condition
	On 	Time
	Time 	76


	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Anim	FX_goblin_fly
		bhvr	behaviors/smother_fly.bhvr
		Magnet	Target
		LookAt	Target


	End


End

Condition
	On	PrimeHit


	Event
		Ename	All
		Type	Destroy
	End

End

End		