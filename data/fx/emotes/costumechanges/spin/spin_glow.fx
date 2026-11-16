#############################################################
## Spin_Glow.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

        Event
                EName   Prime
                Type    local
                At      Root
                Geom    Dummy02
        End

        Event
                EName   Ring1
                Type    local
                At      Prime
                bhvrOverride
			Spin		0 .1 0
			SpinJitter	0 .05 0
		End
                Part4   POWERS\SuperSpeed\Leaves.part
                Part1   POWERS\SuperSpeed\Ensnare.part
                Part3   POWERS\SuperSpeed\EnsnareMist.part
                Part5   POWERS\SuperSpeed\EnsnareMist2.part
        End
End

Condition
        On      Time
        Time    20

        Event
                Type    Local
                At      Prime
		AltPiv	1
                Sound whirly_loop 60 60 .42
        End

        Event
                Type    Local
                At      Prime
		AltPiv	2
                Sound hasten 80 80 .7
        End

        Event
                Type    local
                At      Prime
                bhvr    behaviors\SuperSpeedWirlwind.bhvr
                Sound Whirlwind2_loop 80 80 .8
        End

        Event
                Ename   SwirlForce
                Type    Local
                At      Root
                BhvrOverride
                        PositionOffset          0 4 0
                        PhysForceType           CWSwirl
                        PhysForceRadius         15
                        PhysForceCentripetal    1.5
                        PhysForcePower          250
                        PhysForcePowerJitter    5
                End
        End

        Event
                Ename   LevitationForce
                Type    Local
                At      Root
                BhvrOverride
                        PhysForceType           Up
                        PhysForceRadius         12
                        PhysForcePower          200
                        PhysForcePowerJitter    5
                End
        End

        Event
                Ename   SpreadForce
                Type    Local
                At      Root
                BhvrOverride
                        PositionOffset          0 7 0
                        PhysForceType           Out
                        PhysForceRadius         6
                        PhysForcePower          30
                        PhysForcePowerJitter    5
                End
        End

        Event
                Ename   Shhot
                Type    Local
                At      Root
                BhvrOverride
                        PositionOffset          0 12 0
                        PhysForceType           Out
                        PhysForceRadius         5
                        PhysForcePower          300
                        PhysForcePowerJitter    5
                End
        End

        Event
                Ename   Gbase
                Type    Local
                At      Root
                BhvrOverride
                        PositionOffset          0 -1 0
                        PhysForceType           Out
                        PhysForceRadius         5
                        PhysForcePower          150
                        PhysForcePowerJitter    5
                End
        End
End

Condition
        On      cycle
        Time    13

        Event
                EName   Rings
                Type    Local
                At      Prime
                Bhvr    behaviors\slowtornadomotion.bhvr
                geom    SpeedWisp
		Lifespan 55
        End
End

#############################################################

EnD