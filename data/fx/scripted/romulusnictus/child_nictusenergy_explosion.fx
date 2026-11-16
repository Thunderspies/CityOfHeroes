#############################################################
## HEADER
#############################################################

FxInfo

lifespan 90

## SOUND FX ######################################################

#Condition
#
#	On Time
#	Time 0
#
#	Event
#		Type	Local
#		At	hips
#		Sound	Nova2 100.0 30.0 .76
#	End
#End

## NOVA FX ######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	hips
		Part2	POWERS\CosmicPowers\Nictus\NictusNova\Inferno.part
		Lifespan 22
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	Posit
		At	HIPS
		Bhvr 	Powers\cosmicpowers\PeaceBringer\Nova\BlastScaller.bhvr
		Geom	CosmicNictusShield
		LifeSpan 16
	End

	Event
		Type	Posit
		At	HIPS
		Bhvr 	Powers\cosmicpowers\PeaceBringer\Nova\BlastScaller.bhvr
		Geom	CosmicNictusShield
		LifeSpan 16
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Primex
		Type	Local
		At	hips
		Bhvr	Behaviors\CameraShake01.bhvr
		BhvrOverride
			Shake          2.0     #How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.0425    #how long shake lasts
			ShakeRadius    128  #(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaTendrils.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaTendrilsBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaTendrilsPurple.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaSparks.part
		Lifespan 27
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	POWERS\CosmicPowers\Nictus\NictusNova\PeaceBringerTest.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr    POWERS\CosmicPowers\Nictus\NictusNova\Infernobubble.bhvr
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaSmokeBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaSmokePurple.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaSmoke.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaTendrils01.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaTendrilsBlue01.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\NovaTendrilsPurple01.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\TendrilBlack.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\TendrilBlue02.part
		Part	POWERS\CosmicPowers\Nictus\NictusNova\TendrilPurple02.part
		Lifespan 27
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	Posit
		At	HIPS
		Bhvr 	Powers\cosmicpowers\PeaceBringer\Nova\BlastScaller.bhvr
		Geom	CosmicNictusShield
		LifeSpan 16
	End

	Event
		EName 	Prime8
		Type	Local
		At	root
		bhvr	POWERS\CosmicPowers\Nictus\NictusNova\BlastRingScale.bhvr
		Part1   POWERS\CosmicPowers\Nictus\NictusNova\Infernoflash.part
		Part2   POWERS\CosmicPowers\Nictus\NictusNova\Infernoflash2.part
	End
End

########################################################

End