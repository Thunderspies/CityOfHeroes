#############################################################
## Child_ProjectileCards.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	ProjectileSpin
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	10 90 10
			Spin		0.0 0.15 0.0
			SpinJitter	0.05 0.05 0.05
			Scale		2.0 2.0 2.0
		End
		Geom	TarotCardRig
	End

	Event
		Type	Local
		At	ProjectileSpin
		AltPiv	1
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

	Event
		Type	Local
		At	ProjectileSpin
		AltPiv	2
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
		part	:CardStreak2.part
	End

	Event
		Type	Local
		At	ProjectileSpin
		AltPiv	3
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
		part	:CardStreak3.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	ProjectileSpinRev
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	10 90 10
			Spin		0.0 -0.15 0.0
			SpinJitter	0.05 0.05 0.05
			Scale		2.0 2.0 2.0
		End
		Geom	TarotCardRig
	End

	Event
		Type	Local
		At	ProjectileSpinRev
		AltPiv	1
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

	Event
		Type	Local
		At	ProjectileSpinRev
		AltPiv	2
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
		part	:CardStreak2.part
	End

	Event
		Type	Local
		At	ProjectileSpinRev
		AltPiv	3
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
		part	:CardStreak3.part
	End
End

#############################################################

End