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
		Bhvr	:HandRotateExplosive.bhvr
		Geom	ExplosiveArrow
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
		Geom	aExplosiveArrow
		Sound bowtwang3 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End
	
End

Condition
	On 	PrimeHit

	Event
		Type	start
		At	T_Root	
		ChildFx  WEAPONS\Bow\Archery\ExplosiveArrow.fx	
		lifeSpan	70
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End

End			