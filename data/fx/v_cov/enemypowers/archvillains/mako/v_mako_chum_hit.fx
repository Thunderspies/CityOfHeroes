#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Lifespan 90


Condition
	On Time
	Time 0
	Event
		Type	Local
		At	Origin 
		Sound ChumHit1 80 80 .99
	End
End

Condition
	On 	Time
	Time 	0
############################################

	Event
		Type	local
		At	UarmR

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	LarmR
		Lifespan	100
	End
	Event
		Type	local
		At	LarmR

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	HandR
		Lifespan	100
	End
	Event
		Type	local
		At	UlegR

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	LlegR
		Lifespan	100
	End


	Event
		Type	local
		At	UarmL

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		pother	LarmL
		Lifespan	100
	End
	Event
		Type	local
		At	LarmL

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	HandL
		Lifespan	100
	End
	Event
		Type	local
		At	UlegL

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	LlegL
		Lifespan	100
	End




End
Condition
	On 	Time
	Time 	0
############################################

	Event
		Type	local
		At	UarmR

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	LarmR
		Lifespan	120
	End
	Event
		Type	local
		At	LarmR

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	HandR
		Lifespan	120
	End
	Event
		Type	local
		At	UlegR

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	LlegR
		Lifespan	120
	End


	Event
		Type	local
		At	UarmL

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	LarmL
		Lifespan	120
	End
	Event
		Type	local
		At	LarmL

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	HandL
		Lifespan	120
	End
	Event
		Type	local
		At	UlegL

		Part    :V_Mako_WaterDrips.part
		Part    :V_Mako_WaterDripsRed.part
		Part    :V_Mako_Chumdrops.part
		pother	LlegL
		Lifespan	120
	End




End

Condition
	On 	Time
	Time 	0


	Event
		ename	splashhead
		Type	positonly
		At	head
		lookat	T_Head
	End

	Event
		Type	local
		At	splashhead
		Part    :V_Mako_ChumSpittleSplash.part
		Part    :V_Mako_ChumSpittleSplashRed.part
		bhvroverride
			positionoffset	0 0 .4
			pyrrotate -90 0 0
		end
		Lifespan	45
		pmagnet	root
	End
	Event
		ename	splasharmR
		Type	positonly
		At	UarmR
		lookat	T_Head
	End

	Event
		Type	local
		At	splasharmR
		Part    :V_Mako_ChumSpittleSplash.part
		Part    :V_Mako_ChumSpittleSplashRed.part
		bhvroverride
			positionoffset	0 0 .4
			pyrrotate -90 0 0
		end
		Lifespan	45
		pmagnet	root
	End
	Event
		ename	splasharmL
		Type	positonly
		At	UarmL
		lookat	T_Head
	End

	Event
		Type	local
		At	splasharmL
		Part    :V_Mako_ChumSpittleSplash.part
		Part    :V_Mako_ChumSpittleSplashRed.part
		bhvroverride
			positionoffset	0 0 .4
			pyrrotate -90 0 0
		end
		Lifespan	45
		pmagnet	root
	End
	Event
		ename	splashlegR
		Type	positonly
		At	LlegR
		lookat	T_Head
	End

	Event
		Type	local
		At	splashlegR
		Part    :V_Mako_ChumSpittleSplash.part
		Part    :V_Mako_ChumSpittleSplashRed.part
		bhvroverride
			positionoffset	0 0 .4
			pyrrotate -90 0 0
		end
		Lifespan	45
		pmagnet	root
	End
	Event
		ename	splashlegL
		Type	positonly
		At	LlegL
		lookat	T_Head
	End

	Event
		Type	local
		At	splashlegL
		Part    :V_Mako_ChumSpittleSplash.part
		Part    :V_Mako_ChumSpittleSplashRed.part
		bhvroverride
			positionoffset	0 0 .4
			pyrrotate -90 0 0
		end
		Lifespan	45
		pmagnet	root
	End

end



End
