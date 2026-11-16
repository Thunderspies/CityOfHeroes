#############################################################
## Veteran Reward Combat Pet - Blue Energy Blast
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	19

	Event
		EName	core
		Type	local
		At	Hips
		Sound PeaceBlast1 100.0 100.0 0.5
		Lifespan 21
	End
End

## PROJECTILE 1 #################################################################

Condition
	On	Time
	Time	19

	Event
		EName	Prime
		Type	start
		At	Hips
		bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Part4	:Blue_Projectile_Glow_01.part
		Part4	:Blue_Projectile_Core_01.part
		Part1	:Blue_Projectile_Bubbles_01.part
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		altpiv	1
		Part2	:Blue_Trail_BubblesColor_01.part
		Part2	:Blue_Trail_BubblesBright_01.part
		Part5	:Blue_Trail_01.part
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	cometParts
		Type	Destroy
	End
End

#############################################################

End