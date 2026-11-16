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
		Part	:Splash_Flash2.part
		Part	:Splash_Main.part
		Part	:Splash_Main_Hilite.part
		Part	:Splash_MainSplatter_Hilight.part
		Part	:Splash_MainSplatter.part
		Lifespan 10
	End
End

#############################################################

End