#########################################################
##	template

FxInfo


Flags InheritAnimScale InheritAlpha

LifeSpan 300


#### arrow in hand  ########################

Condition
	On	Time
	Time	35

	Event	
		Type	Local
		At	WepR
		Sound arrowout2 50 50 .25
	End
End



Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotateFlame.bhvr
		Geom	BluntArrow
		LifeSpan 46
	End
End

####  projectile arrow  ##############################

Condition
	On 	Time
	Time 	81

	
	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aBluntArrow
		Sound bowtwang3 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End
	
End

Condition
	On 	PrimeHit

	Event
		Type	startPositOnly
		At	T_Chest
		Bhvr	:StunArrowHit.bhvr
		Geom	BluntArrow
		LifeSpan 46
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End

End			



		