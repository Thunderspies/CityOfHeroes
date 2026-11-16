#############################################################
## SoulCatcher_Beam.fx
#############################################################

FxInfo
Lifespan 300

#########################################################

Condition
	On 	Time
	Time 	0



	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\FadeInOut.bhvr
		BhvrOverride
			PositionOffset	-0.2 -0.2 0
			Scale		0.05 0.05 0.05
			PyrRotate	-270 115 -50
		End
		Geom	FX_SoulCatcher_Gem
		Lifespan 90
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Type	Local
		At	Root
		Sound 	Soulcatcher_Beam_01 100 100 .8
	End

	Event
		Type	Local
		At	WepL
		Part	:GemGlow.part
		Lifespan 25
	End
End

Condition
	On 	Time
	Time 	22


	Event
		EName 	Prime
		Type	Start
		At	WepL
		Part	:Beam.part
		Part	:Beam_EdgeNoise.Part
		Part	:Tendril_Stretch.part
		Bhvr	behaviors\Braz\Projectile_Fast_01.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime2
		Type	Start
		At	WepL
		Magnet	Target
		LookAt	Target
		Bhvr	behaviors\Braz\Projectile_Fast_01.bhvr
		Part	:Noise_Ball.part
		Part	:Tendril_Ball.part
	End
End

Condition
	On 	Time
	Time 	22

	Event
		EName 	Prime3
		Type	Start
		At	WepL
		Magnet	Target
		LookAt	Target
		Bhvr	behaviors\Braz\Projectile_Fast_01.bhvr
		Part	:Tendril_Spread.part
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Prime2
		Type	Destroy
	End

	Event
		EName 	Prime3
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	50

	Event
		EName 	Hand1
		Type	Destroy
	End

	Event
		EName 	Hand2
		Type	Destroy
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Ename	SoulTrap
		Type	Start
		At	T_Chest
		Part	:SoulTrail_Idle.part
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	55

	Event
		Ename	SoulTrap
		Type	Start
		At	T_Chest
		Bhvr	behaviors\Braz\Projectile_FastWithUp_01.bhvr
		Magnet	WepL
		LookAt	WepL
		Part	:SoulTrail.part
		Lifespan 20
	End
End

#########################
End