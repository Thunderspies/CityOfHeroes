#########################################################
##	template

FxInfo


Flags InheritAnimScale InheritAlpha

LifeSpan 300

######## arrow in hand   ###########################

Condition
	On	Time
	Time	25

	Event	
		Type	Local
		At	WepR
		Sound arrowout2 50 50 .25
	End
End


Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate.bhvr
		Geom	NormalArrow
		LifeSpan 17
	End
End

######## arrow projectile  ##########################
Condition
	On 	Time
	Time 	43

	
	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		Sound bowtwang3 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End

End

##################################


Condition
	On 	PrimeHit

#	Event
#		EName	Fire2
#		Type	Local
#		At	T_chest
#			
#		BHVR	Powers/Bow/ArrowHit.bhvr
#		Geom	NormalArrowHit
#		
#		Lifespan	44
#
#
#	End
#
	Event
		Ename 	Prime
		Type	Destroy
	End
End


End

		