#############################################################
## Child_ProjectileCards.fx
#############################################################

FxInfo

Lifespan 150

#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	ProjectileSpin
		Type	PositOnly
		At	Chest
		Bhvr	:CardRigShrink.bhvr
		BhvrOverride
			PyrRotateJitter	10 90 10
			Spin		0.0 0.3 0.0
			SpinJitter	0.05 0.05 0.05
		End
		Geom	TarotCardRig
	End

	Event
		Type	Local
		At	ProjectileSpin
		AltPiv	1
		Bhvr	:CardsFadeOut.bhvr
		Geom	TarotCard_Death
		Geom	TarotCard_TheTower
		Geom	TarotCard_TheFool
		Geom	TarotCard_TheHermit
		Geom	TarotCard_TheLovers
		Geom	TarotCard_TheMagician
		Geom	TarotCard_TheSun
		Geom	TarotCard_TheMoon
		Geom	TarotCard_WheelOfFortune
		Lifespan 10
	End

	Event
		Type	Local
		At	ProjectileSpin
		AltPiv	2
		Bhvr	:CardsFadeOut.bhvr
		Geom	TarotCard_Death
		Geom	TarotCard_TheTower
		Geom	TarotCard_TheFool
		Geom	TarotCard_TheHermit
		Geom	TarotCard_TheLovers
		Geom	TarotCard_TheMagician
		Geom	TarotCard_TheSun
		Geom	TarotCard_TheMoon
		Geom	TarotCard_WheelOfFortune
		Lifespan 10
	End

	Event
		Type	Local
		At	ProjectileSpin
		AltPiv	3
		Bhvr	:CardsFadeOut.bhvr
		Geom	TarotCard_Death
		Geom	TarotCard_TheTower
		Geom	TarotCard_TheFool
		Geom	TarotCard_TheHermit
		Geom	TarotCard_TheLovers
		Geom	TarotCard_TheMagician
		Geom	TarotCard_TheSun
		Geom	TarotCard_TheMoon
		Geom	TarotCard_WheelOfFortune
		Lifespan 10
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	ProjectileSpinRev
		Type	PositOnly
		At	Chest
		Bhvr	:CardRigShrink.bhvr
		BhvrOverride
			PyrRotateJitter	10 90 10
			Spin		0.0 -0.3 0.0
			SpinJitter	0.05 0.05 0.05
		End
		Geom	TarotCardRig
	End

	Event
		Type	Local
		At	ProjectileSpinRev
		AltPiv	1
		Bhvr	:CardsFadeOut.bhvr
		Geom	TarotCard_Death
		Geom	TarotCard_TheTower
		Geom	TarotCard_TheFool
		Geom	TarotCard_TheHermit
		Geom	TarotCard_TheLovers
		Geom	TarotCard_TheMagician
		Geom	TarotCard_TheSun
		Geom	TarotCard_TheMoon
		Geom	TarotCard_WheelOfFortune
		Lifespan 10
	End

	Event
		Type	Local
		At	ProjectileSpinRev
		AltPiv	2
		Bhvr	:CardsFadeOut.bhvr
		Geom	TarotCard_Death
		Geom	TarotCard_TheTower
		Geom	TarotCard_TheFool
		Geom	TarotCard_TheHermit
		Geom	TarotCard_TheLovers
		Geom	TarotCard_TheMagician
		Geom	TarotCard_TheSun
		Geom	TarotCard_TheMoon
		Geom	TarotCard_WheelOfFortune
		Lifespan 10
	End

	Event
		Type	Local
		At	ProjectileSpinRev
		AltPiv	3
		Bhvr	:CardsFadeOut.bhvr
		Geom	TarotCard_Death
		Geom	TarotCard_TheTower
		Geom	TarotCard_TheFool
		Geom	TarotCard_TheHermit
		Geom	TarotCard_TheLovers
		Geom	TarotCard_TheMagician
		Geom	TarotCard_TheSun
		Geom	TarotCard_TheMoon
		Geom	TarotCard_WheelOfFortune
		Lifespan 10
	End
End

#############################################################

End