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
		Part	:DarkTendrilsEmber.part
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
		Anim	FX_darktenticle_B
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Neck
		Bhvr	behaviors\TentacleFade.bhvr
		Anim	FX_darktenticle
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Head
		Bhvr	behaviors\TentacleFade.bhvr
		Anim	FX_darktenticle_A
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Hair
		Bhvr	behaviors\TentacleFade.bhvr
		Anim	FX_darktenticle
	End

	Event
		Type	Local
		At	Prime
		AnimPiv Waist
		Bhvr	behaviors\TentacleFade.bhvr
		Anim	FX_darktenticle_A
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
		Anim	FX_darktenticle_B
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Neck
		Bhvr	behaviors\TentacleFade.bhvr
		Anim	FX_darktenticle
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Head
		Bhvr	behaviors\TentacleFade.bhvr
		Anim	FX_darktenticle_A
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Hair
		Bhvr	behaviors\TentacleFade.bhvr
		Anim	FX_darktenticle
	End

	Event
		Type	Local
		At	Prime2
		AnimPiv Waist
		Bhvr	behaviors\TentacleFade.bhvr
		Anim	FX_darktenticle_A
	End
End

#############################################################

End