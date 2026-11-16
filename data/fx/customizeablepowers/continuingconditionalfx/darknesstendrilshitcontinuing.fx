#############################################################
## DarknessTendrilsHitContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	TriggerBits
	TriggerBits	NONHUMAN

	Event
		Type	Destroy
		EName	ALL
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	posit
		At	root
		Sound tentacle_loop 80 80 0.6
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End

	Event

		ENAME	Base
		Type	posit
		At	root
		Part	:TendrilPuddle.part
		Part	:TendrilPuddle2.part
		Part	:DarkTendrilRing.Part
		Part	:DarkTendrilsEmber.part
		Part	:DarkTendrilsEmber_Alpha.part
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
		BhvrOverride
			TintGeom		1
		End
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

##############################################################
##Tenticles02
######################################################

Condition
	On	Time
	Time	10

	Event
		EName 	Prime2
		Type	Local
		At	Root
		Bhvr	behaviors\ScaleTenticleOutside.bhvr
		BhvrOverride
			TintGeom		1
		End
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