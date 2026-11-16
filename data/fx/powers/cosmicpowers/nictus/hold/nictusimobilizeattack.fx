#########################################################
## Umbral Blast - Gravimetric Snare
#########################################################

FxInfo

LifeSpan	100

## SOUND FX ######################################################

Condition
	On	Time
	Time	15
	
	Event
		Type	Local
		At	WepL
		Sound Warshadeimmobilizer 80 80 .8
	End
End

## EYE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Eyes
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyes.fx
	End
End

## HANDS ######################################################

Condition
	On	Time
	Time	15
	
	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandStreaksBlue.part
		LifeSpan 5
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandStreaksBlue.part
		LifeSpan 5
	End
End

## PROJECTILE ######################################################

Condition
	On	Time
	Time	20

	Event
		EName	Prime
		Type	start
		At	mystic
		Bhvr	:Imobilize.bhvr
		Part	:BLKTendrils.part
		Part	:BLKTendrilsTrail.part
		Part	:Light.part
		Part	:PurpleTendrils.part
		Part	:BlueTendrils.part
		Part	:PurpleTendrilsTrail.part
		Part	:BlueTendrilsTrail.part
		Part	:Sparks.part
		Magnet	T_Root
		Lookat	T_Root
	End
End

## CLEAN UP ######################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

########################################################

End			