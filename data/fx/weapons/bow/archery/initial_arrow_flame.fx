#########################################################
##	template

FxInfo


Flags InheritAnimScale InheritAlpha

LifeSpan 300


#### arrow in hand  ########################

Condition
	On	Time
	Time	5

	Event	
		Type	Local
		At	WepR
		Sound arrowout3 50 50 .32
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	arrow
		Type	Local
		At	WepR
		Bhvr	:HandRotateFlame.bhvr
		Geom	HighDamageArrow
		LifeSpan 46
	End
	
	Event
		EName 	fire
		Type	Local
		At	arrow
		altpiv	1
		Part	:fire.part
		Part	:fire.part
		Part	:fire.part
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
		Geom	aHighDamageArrow
		Sound bowtwang3 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	fire1
		Type	Local
		At	Prime
		altpiv	1
		Part	:fire.part
		Part	:fire.part
		Part	:fire.part
		
	End
	
End

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

	Event
		Ename 	Prime
		Type	Destroy
	End
End



End		