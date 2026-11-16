#########################################################
## Staff of the M.A.G.I. - Magic Blast (Initial Delayed)
#########################################################

FxInfo

LifeSpan	200

## SOUND FX #######################################################

Condition
	
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Geom	Staff_06	##This geom needed##	
		#LifeSpan 45
	End
End

## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	StaffEnd
		Type	Local 
		At	WepR
		BhvrOverride
			PositionOffset	0 0 2
		End
	End
End

Condition
	On	Time
	Time	24

	Event
		EName 	Prime
		Type	Start 
		At	StaffEnd
		Part2	POWERS\DarknessControl\BeamRapidTenticles.part
		Part1	POWERS\DarknessControl\DarkBlastPuddleTrail.Part
		Part3	POWERS\DarknessControl\BeamRapidCloud2.part
		Part4	POWERS\DarknessControl\MoonBeamEmber.part
		Part5	POWERS\DarknessControl\MoonBeamEmberLarge.part
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime2
		Type	Start 
		At	StaffEnd
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		Part1	POWERS\DarknessControl\MoonBeamRapidCloud.part
		Part2	POWERS\DarknessControl\BeamRapidTenticlesHead.part
		Part3	POWERS\DarknessControl\DarkBlastTendrill01.part
		Part4	POWERS\DarknessControl\DarkBlastTendrill03.part
	End

	Event
		EName 	Prime3
		Type	Start 
		At	StaffEnd
		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		Part2	POWERS\DarknessControl\DarkBlastTendrill02.part
		Part3	POWERS\DarknessControl\DarkBlastTendrill04.part
	End
End

## ON HIT #######################################################

Condition
	On 	PrimeHit

	Event
		EName 	HitFake
		Type	local 
		At	target
		Part1	POWERS\DarknessControl\Blast.part
		Part2	POWERS\DarknessControl\Blast.part
	End
	
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

#########################################################

End