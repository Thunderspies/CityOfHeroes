#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR

		Part2	:Muzzle_Tendril.part
		Part2	:Muzzle_Tendril_Streak.part
		Part2	:Muzzle_Smoke.part
		PMagnet	WepR
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	HandGlowRight2
		Type	Local
		At	WepR
		Part1	:Muzzle_Blast_Rings.part
		PMagnet	WepR
		Lifespan 25

	End

	Event
		EName 	HandGlowRight2
		Type	Local
		At	WepR
		Part1	:Muzzle_Tendril_Core.part
		Part2	:Muzzle_Glow.part
		PMagnet	WepR
		Lifespan 30

	End
End

#############################################################

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Part	:Muzzle_Flash.part
	End

	Event
		EName 	Prime
		Type	Start
		At	WepR

		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0
		End

		Part	:Projectile_Head_Ember.part
		Part	:Projectile_Trail_Tendril.part
		Part	:Projectile_Trail_Rings.part
		Part	:Projectile_Trail_Steam.part

		Magnet	Target
		LookAt	Target
		Lifespan	12
	End
End


Condition
	On 	PrimeHit

	Event
		Type	Local
		At	T_Chest
		ChildFX	:Android_Plasma_ChargedBlast_Hit.fx
	End


	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End