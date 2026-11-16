#############################################################
## ChockingSporesHit.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	Head
		Part	EnemyFx\DevouringEarth\Mushrooms\ChockingSporesHitDots.part
		Part	EnemyFx\DevouringEarth\Mushrooms\ChockingSporesHit.part
		Part	EnemyFx\DevouringEarth\Mushrooms\ChockingSporesHit2.part
		PMagnet	Head
		Sound mushroomhit3_loop 40 40 0.3
	End
End

#############################################################

End