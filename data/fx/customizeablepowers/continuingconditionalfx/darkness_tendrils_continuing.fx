#############################################################
## DarknessTendrilsHitContinuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	posit
		At	root
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Sound tentacle_loop 80 80 0.6
		Lifespan 420
	End

	Event
		ENAME	Base
		Type	posit
		At	root
		Part1	POWERS\DarknessControl\TendrilPuddle.part
		Part2	POWERS\DarknessControl\TendrilPuddle2.part
		Part3	POWERS\DarknessControl\DarkTendrilRing.Part
		Part5	POWERS\DarknessControl\DarkTendrilsEmber.part
		Sound grexplo2 80 80 0.6
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prime
		Type	Local
		At	Root
		Bhvr	behaviors\ScaleTenticle.bhvr
		Anim	FX_tenticlebase
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Chest
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacleB
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Neck
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacle
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Head
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacleA
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Hair
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacle
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Waist
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacleA
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName 	Prime2
		Type	Local
		At	Root
		Bhvr	behaviors\ScaleTenticleOutside.bhvr
		Anim	FX_tenticlebase
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Chest
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacleB
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Neck
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacle
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Head
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacleA
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Hair
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacle
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Waist
		Bhvr	behaviors\TentacleFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_Tintable_DarkTentacleA
	End
End

#############################################################

End