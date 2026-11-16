#############################################################
##
#############################################################

FXInfo

## PROJECTILE ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type	Start
		At	Origin
		BhvrOverride
			InitialVelocityJitter	0.5 0.5 0.5
			TrackRate		1.25
		End
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Card
		Type	Local
		At	Prime
		Bhvr	:ProjectileCards.bhvr
		Geom	TarotCard_Death
		Geom	TarotCard_TheTower
		Geom	TarotCard_TheFool
		Geom	TarotCard_TheHermit
		Geom	TarotCard_TheLovers
		Geom	TarotCard_TheMagician
		Geom	TarotCard_TheSun
		Geom	TarotCard_TheMoon
		Geom	TarotCard_WheelOfFortune
		part	:CardStreak1.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End