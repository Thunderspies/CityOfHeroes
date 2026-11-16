#############################################################
## SeedHamidon_Aura.fx
#############################################################

FxInfo
#Lifespan 200

#############################################################


Condition
	On 	Cycle
	Time 	15

	Event
		Ename	HeadAnchor
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
	#	Part	:Splash_Flash.part
		Part	:TailSplash.part
		Part	:TailSplash_Hilite.part
		Part	:Splash_TailSplatter_Hilight.part
		Part	:Splash_TailSplatter.part
		Lifespan 10
	End
End

#############################################################

End