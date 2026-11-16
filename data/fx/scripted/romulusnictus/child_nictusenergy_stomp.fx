#############################################################
## FX System Name
#############################################################

FxInfo

Lifespan 90

#############################################################

#Condition
#
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	hips
#		Sound	footstomp3 90.0 30.0 1.0
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		bhvr	POWERS\CosmicPowers\PeaceBringer\FootStomp\BlastRingScale.bhvr
		Part1   Scripted\RomulusNictus\Infernoflash.part
		Part2   Scripted\RomulusNictus\Infernoflash2.part
		LifeSpan 5
	End

	Event
		Type	Start
		At	Origin
		Bhvr	POWERS\CosmicPowers\PeaceBringer\FootStomp\FootStompCameraShake.bhvr
		BhvrOverride
			Shake          1.5
			ShakeFallOff   0.0625
			ShakeRadius    128
		End
		LifeSpan 10
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	start
		At	Origin
		Part	Scripted\RomulusNictus\Stomp_Tendrils_Black.part
		Part	Scripted\RomulusNictus\Stomp_Tendrils_Blue.part
		Part	Scripted\RomulusNictus\Stomp_Tendrils_Violet.part
		Part	Scripted\RomulusNictus\Stomp_Motes1.part
		Part	Scripted\RomulusNictus\Stomp_Motes2.part
		LifeSpan 25
	End

	Event
		Type	start
		At	Origin
		Bhvr	Scripted\RomulusNictus\FootStompCracks_Additive.bhvr
		Splat	ImpactCracks.tga
		Lifespan 1
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Origin
		Bhvr	POWERS\CosmicPowers\PeaceBringer\FootStomp\PeaceBringerTest.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr    POWERS\CosmicPowers\PeaceBringer\FootStomp\Infernobubble.bhvr
		Part	Scripted\RomulusNictus\Stomp_RingSmoke_Black.part
		Part	Scripted\RomulusNictus\Stomp_RingSmoke_Blue.part
		Part	Scripted\RomulusNictus\Stomp_RingSmoke_Violet.part
		Part	Scripted\RomulusNictus\Stomp_RingTendrils_Black.part
		Part	Scripted\RomulusNictus\Stomp_RingTendrils_Blue.part
		Part	Scripted\RomulusNictus\Stomp_RingTendrils_Violet.part
		LifeSpan 5
	End
End

#############################################################

End