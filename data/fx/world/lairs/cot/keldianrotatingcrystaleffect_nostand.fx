#############################################################
## KeldianRotatingCrystalEffect_NoStand.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Crystal_Stand
		Type	Local
		At	root
		BhvrOverride
			Alpha	1
		End
		Geom	Crystal_Stand
		sound	warshadecrystalexplo 100 90 .92
	End

	Event
		EName 	P
		Type	Local
		At	Crystal_Stand
		altpiv	2
		Geom	Keldian_BlackCrystal
		Bhvr	behaviors\KeldianRune_spinFast.bhvr
		#Sound	Crystal3_loop 70 70 .9
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	P2
		Type	Local
		At	Crystal_Stand
		altpiv	2
		Part	:WarshadeTendrilBlack.part
		Part	:WarshadeTendrilPurple.part
		Part3	:WarshadeTendrilBlue.part
		Sound nictuscrystal_loop 80 80 .91
	End
End

#############################################################

Condition
	On 	Time
	Time 	1
	Chance	.8
	Event
		EName 	Shine1
		Type	Local
		At	P
		AltPiv	1
		Part1	:ShineCoreFastSML.part
		Part2	:ShineRingsFastSML.part
	End
End

Condition
	On 	Time
	Time 	7
	Chance	.3

	Event
		EName 	Shine2
		Type	Local
		At	P
		AltPiv	2
		Part1	:ShineCoreFastSML.part
		Part2	:ShineRingsFastSML.part
	End
End

Condition
	On 	Time
	Time 	2
	Chance	.34

	Event
		EName 	Shine3
		Type	Local
		At	P
		AltPiv	3
		Part1	:ShineCoreFastSML.part
		Part2	:ShineRingsFastSML.part
	End
End

Condition
	On 	Time
	Time 	1
	Chance	.46

	Event
		EName 	Shine4
		Type	Local
		At	P
		AltPiv	4
		Part1	:ShineCoreFastSML.part
		Part2	:ShineRingsFastSML.part
	End
End

Condition
	On 	Time
	Time 	0
	Chance	.36

	Event
		EName 	Shine5
		Type	Local
		At	P
		AltPiv	5
		Part1	:ShineCoreFastSML.part
		Part2	:ShineRingsFastSML.part
	End

End

Condition
	On 	Time
	Time 	7
	Chance	.41

	Event
		EName 	Shine6
		Type	Local
		At	P
		AltPiv	6
		Part1	:ShineCoreFastSML.part
		Part2	:ShineRingsFastSML.part
	End
End

Condition
	On 	Time
	Time 	9
	Chance	.3

	Event
		EName 	Shine7
		Type	Local
		At	P
		AltPiv	7
		Part1	:ShineCoreFastSML.part
		Part2	:ShineRingsFastSML.part
	End

End

Condition
	On 	Time
	Time 	8
	Chance	.22

	Event
		EName 	Shine8
		Type	Local
		At	P
		AltPiv	8
		Part1	:ShineCoreFastSML.part
		Part2	:ShineRingsFastSML.part
	End

End

#############################################################

Condition
	On 	death

	Event
		EName 	Shine1
		Type	Destroy
	End

	Event
		EName 	Shine2
		Type	Destroy
	End

	Event
		EName 	Shine3
		Type	Destroy
	End

	Event
		EName 	Shine4
		Type	Destroy
	End

	Event
		EName 	Shine5
		Type	Destroy
	End

	Event
		EName 	Shine6
		Type	Destroy
	End

	Event
		EName 	Shine7
		Type	Destroy
	End

	Event
		EName 	Shine8
		Type	Destroy
	End
End

#############################################################

Condition
	On 	death

	Event
		Type	Local
		At	Crystal_Stand
		altpiv	2
		Bhvr 	Powers\cosmicpowers\PeaceBringer\Nova\BlastScaller2.bhvr
		Geom	CosmicNictusShield
		LifeSpan 10	#16
	End

	Event
		Type	Local
		At	Crystal_Stand
		altpiv	2
		Bhvr 	Powers\cosmicpowers\PeaceBringer\Nova\BlastScaller2.bhvr
		Geom	CosmicNictusShield
		LifeSpan 10	#16
	End

	Event
		EName 	Primex
		Type	Local
		At	Crystal_Stand
		altpiv	2
		Bhvr	Behaviors\CameraShake01.bhvr
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaTendrils.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaTendrilsBlue.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaTendrilsPurple.part
		#Part	Powers\cosmicpowers\Nictus\NictusNova\NovaSparks.part
		Lifespan 17	#20
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Crystal_Stand
		altpiv	2
		#BHVR	:CrystalOffsetDeath.bhvr
		Bhvr	Powers\cosmicpowers\Nictus\NictusNova\PeaceBringerTest.bhvr
		LifeSpan	13
	End

	Event
		EName 	Prime1
		Type	Local
		At	Crystal_Stand
		altpiv	2
		#BHVR	:CrystalOffsetDeath.bhvr
		bhvr    Powers\cosmicpowers\Nictus\NictusNova\Infernobubble2.bhvr
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaSmokeBlue.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaSmokePurple.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaSmoke.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaTendrils01.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaTendrilsBlue01.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\NovaTendrilsPurple01.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\TendrilBlack.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\TendrilBlue02.part
		Part	Powers\cosmicpowers\Nictus\NictusNova\TendrilPurple02.part
		LifeSpan	13
	End

	Event
		Type	Local
		At	Crystal_Stand
		altpiv	2
		Bhvr 	Powers\cosmicpowers\PeaceBringer\Nova\BlastScaller2.bhvr
		Geom	CosmicNictusShield
		LifeSpan	13	#16
	End

	Event
		EName 	p2
		Type	Destroy
	End

	Event
		EName 	p
		Type	Destroy
	End

	Event
		Type	Local
		At	Crystal_Stand
		altpiv	2
		BHVR	:CrystalOffsetDeath.bhvr
		Part	:KeldianCrystalChunks.part
		Part	:KeldianCrystalChunks2.part
		Part	:KeldianCrystalSparklies.part
		Part	:KeldianCrystalPurpleSparklies.part
		Part	:KeldianCrystalBlueSparklies.part
	End
End

#############################################################

Condition
	On 		triggerbits
	Triggerbits	death

################ If you want the guy to turn invisible right as he dies AND play particles, do this on triggerbits death
################ (you can't set TicksToLingerAfterDeath to less than about 15 if you want to have a
################ deathFx because he'll be dead before the fx happens.)

	Event
		EName 	Crystal_Stand
		Type	Destroy
	End
End

#############################################################

End