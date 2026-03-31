#set page(
  margin: (top: 3cm, bottom: 2.5cm, left: 3cm, right: 3cm),
)
#set text(
  font: "Arial", 
  lang: "de",
)

// Logo Bereich
#align(center)[
  #image("pictures/logo_hsos.svg", width: 80%)
]

#v(4cm)

// Titelbereich
#align(center)[
  #text(size: 24pt, weight: "bold")[Dokumentation der Implementierung des Spiels Tic-Tac-Toe auf dem \ Nucleo-L412KB]
]

#v(5.5cm)

// Informationsblock
#align(center)[
  #block(width: 90%)[
    #set align(left)
    #grid(
      columns: (4cm, 1fr),
      row-gutter: 1.2em,
      [#strong[Projekt:]], [Implementierung von Tic-Tac-Toe auf einem Nucleo-Board mit Steuerung über UART und Tastatur sowie Ausgabe über LCD und LED-Ring.],
      
      [#strong[Abgabetermin:]], [01. April 2026],
      
      [#strong[Modul:]], [Eingebettete Systeme],
      
      [#strong[Dozent:]], [Prof. Dr.-Ing. Steffen Greiser, \ Dieter Jönen],
      
      [#strong[Studiengang:]], [Wirtschaftsinformatik],
      
      [#strong[Team:]], [
        Aleksandr Leongard (1080072) \
        Kevin Mekelburg (1077281)
      ],
    )
  ]
]

#pagebreak()

// Inhaltsverzeichnis
#outline(
  title: [Inhaltsverzeichnis],
  indent: auto,
)

#pagebreak()

// Seitenzahlen ab dem Inhalt aktivieren
#set page(numbering: "1")
#counter(page).update(1)


= Hardwaresetup und Konfiguration
Dieses Kapitel dokumentiert die physikalische Verschaltung sowie die softwareseitige Initialisierung der Peripherie. Weitere allgemeine Spezifikationen zu den genutzten Hardwarekomponenten und dem Mikrocontroller sind den Veranstaltungsunterlagen zu entnehmen.

== Verdrathung und Pin-Setup 

Die physische Zuordnung der STM32-Peripheriefunktionen zu den verwendeten Hardwarekomponenten ist entscheidend für die spätere softwareseitige Initialisierung. Die folgende Tabelle fasst alle Verbindungen zusammen. Dabei wird der interne Port-Pin Name sowie der auf der Nucleo-32 Platine aufgedruckte Bezeichner angegeben.

#align(center)[
  #set text(size: 10pt)
  #table(
    columns: (auto, auto, 1fr),
    inset: 8pt,
    align: horizon,
    fill: (row, col) => if row == 0 { white } else { white },
    [*MCU Pin (Nucleo Label)*], [*HAL / Peripheriefunktion*], [*Komponente und Bemerkung*],
    [PA2 (VCP_TX)], [USART2_TX (Asynchron)], [UART (Serielle Ausgabe an PC)],
    [PA15 (VCP_RX)], [USART2_RX (Asynchron)], [UART (Serielle Eingabe vom PC)],
    [PA9 (A4)], [I2C1_SCL], [Midas I2C-LCD 2x8 (Takt)],
    [PA10 (A5)], [I2C1_SDA], [Midas I2C-LCD 2x8 (Daten)],
    [PB4 (D12)], [SPI1_MISO (Receive-Only Master)], [Keypad Touch-Sensor (Daten / SDO)],
    [PA1 (A1)], [SPI1_SCK (Receive-Only Master)], [Keypad Touch-Sensor (Takt / SCL)],
    [PA0 (A0)], [TIM2_CH1 (PWM via DMA)], [LED-Ring (WS2812B Datenleitung)],
    [PB3 (LD3)], [GPIO_Output], [On-Board Nutzer-LED (LD3)],
    [+3V3 (+3V3)], [VCC], [Stromversorgung LCD, Keypad, LED-Ring],
    [GND (GND)], [GND], [Gemeinsames Bezugspotenzial]
  )
  #text(size: 10pt, style: "italic")[Tabelle 1: Verdrahtung und Pin-Belegung der Hardwarekomponenten.]
]

#v(1em)

Die physikalische Umsetzung auf dem Steckbrett und die Verlegung der Jumper-Wire Leitungen sind in Abbildung 1 dargestellt.

#align(center)[
  #block(stroke: 1pt + luma(100), inset: 5pt)[
    #image("pictures/aufbau.jpeg", width: 80%)
  ]
  #text(size: 10pt, style: "italic")[Abbildung 1: Foto des physischen Aufbaus am Nucleo-Board auf einem Steckbrett.]
]

Zum besseren Verständnis der softwareseitigen Peripheriekonfiguration ist in Abbildung 2 zusätzlich der Pinout-View der STM32CubeIDE dargestellt.

#align(center)[
  #block(stroke: 1pt + luma(100), inset: 5pt)[
    #image("pictures/ioc.jpeg", width: 60%)
  ]
  #text(size: 10pt, style: "italic")[Abbildung 2: Screenshot der Pinout-Konfiguration in der .ioc-Datei des Projekts.]
]

== Konfiguration der Peripherie 
(Übersicht der verwendeten STM32-Ressourcen: I2C1, USART2, Taktung)

// TODO: ab hier die Vorgängerkapitel auf Plausibilität checken.
= Architektur

= Programmablaufdiagram

= Bedieung des Spieles






